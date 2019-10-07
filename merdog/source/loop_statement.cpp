/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/loop_statement.hpp"
#include "../include/namespace.hpp"
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
#include "../include/if.hpp"
#include <ctime>
#include "../include/environment.hpp"
namespace Mer
{
	extern std::vector<ParserNode*> *current_ins_table;
	extern std::vector<size_t *> _pcs;
	namespace Parser
	{
#pragma region NEW

		void public_part()
		{
			while (token_stream.this_tag() != END)
			{
				switch (token_stream.this_tag())
				{
				case IF:
					build_if();
					break;
				case BREAK:
					token_stream.match(BREAK);
					token_stream.match(SEMI);
					current_ins_table->push_back(new Goto(_pcs.back(), loop_end()));
					break;
				case CONTINUE:
					token_stream.match(CONTINUE);
					token_stream.match(SEMI);
					current_ins_table->push_back(new Continue(_pcs.back(), loop_end()));
					break;
				case WHILE:
					build_while();
					break;
				case DO:
					do_while();
					break;
				default:
					current_ins_table->push_back(get_node());
					break;
				}
			}
		}
		void do_while()
		{
			PosPtr start_pos = std::make_shared<size_t>(current_ins_table->size());
			PosPtr end_pos = std::make_shared<size_t>(0);
			// register the start_pos and end_pos to environment
			new_loop(start_pos, end_pos);
			token_stream.match(DO);
			// new block
			mem.new_block();
			this_namespace->sl_table->new_block();
			token_stream.match(BEGIN);
			public_part();
			token_stream.match(END);
			token_stream.match(WHILE);
			token_stream.match(LPAREN);
			Expr* node = new Expr();
			token_stream.match(RPAREN);

			current_ins_table->push_back(new IfTrueToAOrB(_pcs.back(), std::make_shared<size_t>(*start_pos), std::make_shared<size_t>(current_ins_table->size() + 1), node));
			*end_pos = current_ins_table->size();
			// delete the record of start_pos and end_pos, cause the two var is thoroughly used.
			end_loop();
			// end_block;
			mem.end_block();
			this_namespace->sl_table->end_block();
			token_stream.match(SEMI);
		}
		void demo()
		{
			current_ins_table = new std::vector<Mer::ParserNode*>;
			_pcs.push_back(new size_t(0));
			do_while();
			for (int i = 0; i < current_ins_table->size(); i += 1)
				 std::cout << "<Line:" + std::to_string(i) + ">" << (*current_ins_table)[i]->to_string() << std::endl;
			current_ins_table->push_back(new NonOp());
			size_t *pc = _pcs.back();
			time_t s = clock();
			for (*pc = 0; *pc < current_ins_table->size(); (*pc)++)
			{
				(*current_ins_table)[*pc]->execute();
			}
			time_t e = clock();
			std::cout << "\ntime:" << (double)(e - s) * 1000 / CLK_TCK << " ms";
		}
		void build_while()
		{
			PosPtr start_pos = std::make_shared<size_t>(current_ins_table->size());
			PosPtr end_pos = std::make_shared<size_t>(0);
			// register the start_pos and end_pos to environment
			new_loop(start_pos, end_pos);
			token_stream.match(WHILE);
			token_stream.match(LPAREN);
			Expr* node = new Expr();
			token_stream.match(RPAREN);
			// new block
			mem.new_block();
			this_namespace->sl_table->new_block();
			token_stream.match(BEGIN);
			current_ins_table->push_back(new IfTrueToAOrB(_pcs.back(), std::make_shared<size_t>(*start_pos + 1), end_pos, node));
			public_part();
			current_ins_table->push_back(new Goto(_pcs.back(), start_pos));
			token_stream.match(END);
			*end_pos = current_ins_table->size();
			// end_block, block;
			end_loop();
			mem.end_block();
			this_namespace->sl_table->end_block();
		}
		void build_if()
		{
			token_stream.match(IF);
			token_stream.match(LPAREN);
			PosPtr end_pos=std::make_shared<size_t>(0);
			Expr* node = new Expr();
			token_stream.match(RPAREN);
			// new block
			mem.new_block();
			this_namespace->sl_table->new_block();
			token_stream.match(BEGIN);
			auto iwjt = new IfWithJmpTable(_pcs.back());
			current_ins_table->push_back(iwjt);
			iwjt->jmp_table.push_back({ node,std::make_shared<size_t>(current_ins_table->size()) });
			public_part();
			// end_block;
			token_stream.match(END);
			mem.end_block();
			this_namespace->sl_table->end_block();
			while (token_stream.this_tag() == ELSE_IF)
			{
				token_stream.match(ELSE_IF);
				token_stream.match(LPAREN);
				auto expr = new Expr();
				token_stream.match(RPAREN);
				mem.new_block();
				this_namespace->sl_table->new_block();
				token_stream.match(BEGIN);
				iwjt->jmp_table.push_back({ expr,std::make_shared<size_t>(current_ins_table->size()) });
				public_part();
				token_stream.match(END);
				mem.end_block();
				this_namespace->sl_table->end_block();
				current_ins_table->push_back(new Goto(_pcs.back(), end_pos));
			}
			if (token_stream.this_tag() == ELSE)
			{
				*end_pos = current_ins_table->size();
				token_stream.match(ELSE);
				mem.new_block();
				this_namespace->sl_table->new_block();
				token_stream.match(BEGIN);
				public_part();
				token_stream.match(END);
				mem.end_block();
				this_namespace->sl_table->end_block();
				current_ins_table->push_back(new Goto(_pcs.back(), std::make_shared<size_t>(current_ins_table->size()+1)));
			}
			else
				*end_pos = current_ins_table->size();
			auto tmp = new LConV(std::make_shared<Mem::Bool>(true), (size_t)Mem::BOOL);
			iwjt->jmp_table.push_back({tmp, end_pos });

		}
		void switch_driver() {
			token_stream.match(SWITCH);
			token_stream.match(LPAREN);
			auto expr = new Expr();
			token_stream.match(RPAREN);
			size_t type = expr->get_type();
			switch (type)
			{
			case Mem::BasicType::INT:
				build_switch<Mem::Int>(expr);
				break;
			case Mem::BasicType::STRING:
				build_switch<Mem::String>(expr);
				break;
			default:
				throw Error("unsupported type to switch");
			}
		}
		template<typename KeyType>
		void build_switch(ParserNode *expr){
			PosPtr end_pos = std::make_shared<size_t>(0);
			auto case_set = new CaseSet<KeyType>(_pcs.back(), expr);
			token_stream.match(BEGIN);
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
#pragma endregion
		Mer::ParserNode * while_statement()
		{
			if (token_stream.this_tag() == DO)
			{
				do_while();
				return nullptr;
			}
			While *ret = new While();
			token_stream.match(WHILE);
			token_stream.match(LPAREN);
			ret->condition = new Expr();
			token_stream.match(RPAREN);
			ret->blo = block();
			return ret;
		}
		ParserNode * for_statement()
		{
			mem.new_block();
			this_namespace->sl_table->new_block();
			For *ret = new For();
			token_stream.match(FOR);
			token_stream.match(LPAREN);
			ret->init = statement();
			ret->condition = new Expr();
			token_stream.match(SEMI);

			ret->step_action = new Expr();

			token_stream.match(RPAREN);
			ret->blo = pure_block();
			this_namespace->sl_table->end_block();
			return ret;
		}
	}
	Mem::Object For::execute()
	{
		blo->new_block();
		for (init->execute();
			std::static_pointer_cast<Mem::Bool>(condition->execute())->_value();
			step_action->execute())
		{
			try
			{
				blo->execute();
			}
			catch (Word c)
			{
				if (c.type == Word::Type::Break)
					break;
				else if (c.type == Word::Type::Continue)
					continue;
			}
		}
		blo->end_block();
		return nullptr;
	}
	Mem::Object While::execute()
	{
		blo->new_block();
		while (std::static_pointer_cast<Mem::Bool>(condition->execute())->_value())
		{
			try
			{
				blo->execute();
			}
			catch (Word c)
			{
				if (c.type == Word::Type::Break)
					break;
				else if (c.type == Word::Type::Continue)
					continue;
			}
		}
		blo->end_block();
		return nullptr;
	}
	Mem::Object Goto::execute()
	{
		// when the execute run over, the index will inc 1 so -1 is neccessary
		*index = *target - 1;
		return nullptr;
	}
	std::string Goto::to_string()
	{
		return "goto " + std::to_string(*target);
	}
	Mem::Object Continue::execute()
	{
		*index = *target - 2;
		return nullptr;
	}
	std::string Continue::to_string()
	{
		return "continue " + std::to_string(*target - 1);
	}
}