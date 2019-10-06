/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/loop_statement.hpp"
#include "../include/namespace.hpp"
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
#include "../include/if.hpp"
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
				case BREAK:
					token_stream.match(BREAK);
					token_stream.match(SEMI);
					current_ins_table->push_back(new Goto(_pcs.back(), loop_end()));
					break;
				case CONTINUE:
					token_stream.match(CONTINUE);
					token_stream.match(SEMI);
					current_ins_table->push_back(new Goto(_pcs.back(), loop_start()));
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
			PosPtr end_pos=std::make_shared<size_t>(0);
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
			
			current_ins_table->push_back(new IfTrueToAOrB(_pcs.back(), std::make_shared<size_t>(*start_pos), std::make_shared<size_t>(current_ins_table->size() + 1),node));
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
			for(int i=0;i<current_ins_table->size();i+=1)
				std::cout<<"<Line:"+std::to_string(i)+">"<< (*current_ins_table)[i]->to_string()<<std::endl;
			current_ins_table->push_back(new NonOp());
			size_t *pc = _pcs.back();
			for (*pc = 0; *pc < current_ins_table->size(); (*pc)++)
			{
				(*current_ins_table)[*pc]->execute();
			}
		}
		void build_while()
		{
			PosPtr start_pos = std::make_shared<size_t>(current_ins_table->size());
			PosPtr end_pos = std::make_shared<size_t>(0);
			token_stream.match(WHILE);
			token_stream.match(LPAREN);
			Expr* node = new Expr();
			token_stream.match(RPAREN);
			// new block
			mem.new_block();
			this_namespace->sl_table->new_block();
			token_stream.match(BEGIN);
			current_ins_table->push_back(new IfTrueToAOrB(_pcs.back(), std::make_shared<size_t>(*start_pos+1), end_pos, node));
			public_part();
			current_ins_table->push_back(new Goto(_pcs.back(), start_pos));
			token_stream.match(END);
			*end_pos = current_ins_table->size();
			// end_block;
			mem.end_block();
			this_namespace->sl_table->end_block();
		}
		void build_if()
		{
			PosPtr start_pos = std::make_shared<size_t>(current_ins_table->size());
			PosPtr end_pos = std::make_shared<size_t>(0);
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
			token_stream.match(END);
			*end_pos = current_ins_table->size();
			// end_block;
			mem.end_block();
			this_namespace->sl_table->end_block();
		}

#pragma endregion
		Mer::ParserNode * while_statement()
		{
			if (token_stream.this_tag ()== DO)
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
		return "goto" + std::to_string(*target);
	}
}