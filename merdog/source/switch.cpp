/*
*			MIT License
*			Copyright (c) 2019 Htto Hu
*/
#include "../include/switch.hpp"
#include "../include/lexer.hpp"
#include "../include/expr.hpp"
#include "../include/memory.hpp"
#include "../include/namespace.hpp"
#include "../include/word_record.hpp"
namespace Mer
{
	class For;
	class If;
	class Block;
	namespace Parser
	{
		ParserNode* while_statement();
		ParserNode* for_statement();
		ParserNode* if_statement();
		ParserNode* statement();
		Block* block();
	}
	template<typename KeyType>
	Mer::Switch<KeyType>::Switch(Expr* _expr) :expr(_expr)
	{
		token_stream.match(BEGIN);
		std::vector<Break*>break_vec;
		size_t current_line = 0;
		bool have_default = false;
		mem.new_block();
		this_namespace->sl_table->new_block();
		while (token_stream.this_tag() != END)
		{
			ParserNode* node;
			switch (token_stream.this_tag())
			{
			case DEFAULT:
				token_stream.match(DEFAULT);
				default_line = current_line;
				have_default = true;
				token_stream.match(COLON);
				continue;
			case CASE:
				token_stream.match(CASE);
				if (typeid(KeyType) == typeid(int))
				{
					int v = Mer::Integer::get_value(token_stream.this_token());
					tag_map.insert({ *(KeyType*)(&v),current_line });
				}
				else if (typeid(KeyType) == typeid(std::string))
				{
					std::string v = Mer::String::get_value(token_stream.this_token());
					tag_map.insert({ *(KeyType*)(&v),current_line });
				}
				else
					throw Error("unsupported case tag");
				token_stream.next();
				token_stream.match(COLON);
				continue;
			case BEGIN:
				node = Parser::block();
				break;
			case FOR:
				node = Parser::for_statement();
				break;
			case DO:
			case WHILE:
				node = Parser::while_statement();
				break;
			case SWITCH:
				node = Parser::switch_statement();
				break;
			case IF:
				node = Parser::if_statement();
				break;
			case BREAK:
				token_stream.match(BREAK);
				node = new Break(&ins_index);
				break_vec.push_back(static_cast<Break*>(node));
				token_stream.match(SEMI);
				break;
			default:
				node = Parser::statement();
				break;
			}
			current_line++;
			ins_table.push_back(node);
		}
		if (!have_default)
		{
			default_line = ins_table.size() - 1;
		}
		for (auto& a : break_vec)
		{
			a->set_end_pos(ins_table.size()-1);
		}
		token_stream.match(END);
		this_namespace->sl_table->end_block();
		mem.end_block();
	}

	template<typename KeyType>
	Mem::Object Switch<KeyType>::execute()
	{
#ifdef SAFE_MOD
		auto value = expr->execute();
		KeyType  result;
		switch (result->get_type())
		{
		case Mem::INT:
			result = std::static_pointer_cast<Mem::Int>(result->get_value());
			break;
		case Mem::STRING:
			result = std::static_pointer_cast<Mem::Double>(result->get_value());
			break;
	}
}
#else
		//This is an UB feature, but most of compilers will be ok. if you find something wrong happened please define SAFE_MOD.
		KeyType result = *(KeyType*)((char*)expr->execute().get() + sizeof(void*));
#endif
		auto map_seeker = tag_map.find(result);
		if (map_seeker == tag_map.end())
		{
			ins_index = default_line;
		}
		else
		{
			ins_index = map_seeker->second;
		}
		for (; ins_index < ins_table.size(); ins_index++)
		{
			ins_table[ins_index]->execute();
		}
		ins_index = 0;
		return nullptr;
}

	ParserNode* Mer::Parser::switch_statement()
	{
		token_stream.match(SWITCH);
		token_stream.match(LPAREN);
		auto expr = new Expr();
		token_stream.match(RPAREN);
		size_t type = expr->get_type();
		switch (type)
		{
		case Mem::BasicType::INT:
			return new Switch<int>(expr);
		case Mem::BasicType::STRING:
			return new Switch<std::string>(expr);
		default:
			throw Error("unsupported type to switch");
		}
	}

}

