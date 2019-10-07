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
	extern std::vector<ParserNode*>* current_ins_table;
	extern std::vector<size_t*> _pcs;
	namespace Parser
	{
#pragma region NEW
		void switch_driver();
		void build_for();
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
				case FOR:
					build_for();
					break;
				case SWITCH:
					switch_driver();
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
			size_t* pc = _pcs.back();
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
			PosPtr end_pos = std::make_shared<size_t>(0);
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
				current_ins_table->push_back(new Goto(_pcs.back(), std::make_shared<size_t>(current_ins_table->size() + 1)));
			}
			else
				*end_pos = current_ins_table->size();
			auto tmp = new LConV(std::make_shared<Mem::Bool>(true), (size_t)Mem::BOOL);
			iwjt->jmp_table.push_back({ tmp, end_pos });

		}
		template<typename KeyType>
		void build_switch(PosPtr& default_pos, std::map<KeyType, PosPtr>& case_set);
		void switch_driver() {
			token_stream.match(SWITCH);
			token_stream.match(LPAREN);
			auto expr = new Expr();
			token_stream.match(RPAREN);
			size_t type = expr->get_type();
			ParserNode* node;
			if (type == Mem::INT)
				node = new IntCaseSet(_pcs.back(), expr);
			else if (type == Mem::STRING)
				node = new StrCaseSet(_pcs.back(), expr);
			else
				throw Error("Unsupported case type!");
			current_ins_table->push_back(node);
			switch (type)
			{
			case Mem::BasicType::INT:
				build_switch<int>(static_cast<IntCaseSet*>(node)->default_pos, static_cast<IntCaseSet*>(node)->jmp_table);
				break;
			case Mem::BasicType::STRING:
				build_switch<std::string>(static_cast<StrCaseSet*>(node)->default_pos, static_cast<StrCaseSet*>(node)->jmp_table);
				break;
			default:
				throw Error("unsupported type to switch");
			}
		}
		template<typename KeyType>
		void build_switch(PosPtr& default_pos, std::map<KeyType, PosPtr>& case_set) {
			PosPtr end_pos = std::make_shared<size_t>(0);
			default_pos = end_pos;
			token_stream.match(BEGIN);
			mem.new_block();
			this_namespace->sl_table->new_block();
			while (token_stream.this_tag() != END)
			{
				ParserNode* node;
				switch (token_stream.this_tag())
				{
				case DEFAULT:
					token_stream.match(DEFAULT);
					token_stream.match(COLON);
					default_pos = std::make_shared<size_t>(current_ins_table->size() - 1);
					break;
				case CASE:
					token_stream.match(CASE);
					if (typeid(KeyType) == typeid(int))
					{
						int v = Mer::Integer::get_value(token_stream.this_token());
						case_set.insert({ *(KeyType*)(&v),std::make_shared<size_t>(current_ins_table->size() - 1) });
					}
					else if (typeid(KeyType) == typeid(std::string))
					{
						std::string v = Mer::String::get_value(token_stream.this_token());
						case_set.insert({ *(KeyType*)(&v),std::make_shared<size_t>(current_ins_table->size() - 1) });
					}
					else
						throw Error("unsupported case tag");
					token_stream.next();
					token_stream.match(COLON);
					continue;
				case DO:
					do_while();
					break;
				case WHILE:
					build_while();
					break;
				case SWITCH:
					switch_driver();
					break;
				case FOR:
					build_for();
					break;
				case IF:
					build_if();
					break;
				case BREAK:
					token_stream.match(BREAK);
					token_stream.match(SEMI);
					current_ins_table->push_back(new Goto(_pcs.back(), end_pos));
					break;
				case CONTINUE:
					token_stream.match(CONTINUE);
					token_stream.match(SEMI);
					current_ins_table->push_back(new Continue(_pcs.back(), loop_end()));
					break;
				default:
					current_ins_table->push_back(statement());
					break;
				}
			}
			token_stream.match(END);
			mem.end_block();
			this_namespace->sl_table->end_block();
			*end_pos = current_ins_table->size();
		}
		/*
			var_part;
			goto t;
		start	    step expr;
		t	if.... x:y
			node;
			node;
			goto start;

		*/
		void build_for()
		{
			PosPtr start_pos = std::make_shared<size_t>(current_ins_table->size());

			PosPtr end_pos = std::make_shared<size_t>(0);
			// register the start_pos and end_pos to environment
			ParserNode* compare_part;
			// new block
			mem.new_block();
			this_namespace->sl_table->new_block();

			new_loop(start_pos, end_pos);
			token_stream.match(FOR);
			token_stream.match(LPAREN);
			if(token_stream.this_tag()!=SEMI)
			{ 
				current_ins_table->push_back(var_decl());
				++*start_pos;
			}
			token_stream.match(SEMI);
			if (token_stream.this_tag() == SEMI)
			{
				compare_part = new LConV(std::make_shared<Mem::Bool>(true), Mem::BOOL);
			}
			else
			{
				compare_part = Expr().root();
			}
			token_stream.match(SEMI);
			if (token_stream.this_tag() != RPAREN)
			{
				++* start_pos;
				current_ins_table->push_back(new Goto(_pcs.back(), std::make_shared<size_t>(*start_pos + 1)));
				// start pos is step
				current_ins_table->push_back(Expr().root());
			}
			token_stream.match(RPAREN);

			token_stream.match(BEGIN);

			current_ins_table->push_back(new IfTrueToAOrB(_pcs.back(), std::make_shared<size_t>(current_ins_table->size()+1), end_pos, compare_part));
			public_part();
			current_ins_table->push_back(new Goto(_pcs.back(), start_pos));
			token_stream.match(END);
			*end_pos = current_ins_table->size();
			// end_block, block;
			end_loop();
			mem.end_block();
			this_namespace->sl_table->end_block();
		}
#pragma endregion
		Mer::ParserNode* while_statement()
		{
			if (token_stream.this_tag() == DO)
			{
				do_while();
				return nullptr;
			}
			While* ret = new While();
			token_stream.match(WHILE);
			token_stream.match(LPAREN);
			ret->condition = new Expr();
			token_stream.match(RPAREN);
			ret->blo = block();
			return ret;
		}
		ParserNode* for_statement()
		{
			mem.new_block();
			this_namespace->sl_table->new_block();
			For* ret = new For();
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
	Mem::Object IntCaseSet::execute()
	{
		auto result = jmp_table.find(Mem::get_raw<int>(expr->execute()));
		if (result == jmp_table.end())
			* pc = *default_pos;
		else
			*pc = *(result->second);
		return nullptr;
	}
	std::string IntCaseSet::to_string()
	{
		return std::string();
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
	Mem::Object StrCaseSet::execute()
	{
		auto result = jmp_table.find(Mem::get_raw<std::string>(expr->execute()));
		if (result == jmp_table.end())
			* pc = *default_pos;
		else
			*pc = *(result->second);
		return nullptr;
	}
}