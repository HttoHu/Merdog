/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/loop_statement.hpp"
#include "../include/namespace.hpp"
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
namespace Mer
{
	namespace Parser
	{
		Mer::ParserNode * while_statement()
		{
			if (token_stream.this_tag ()== DO)
			{
				DoWhile* do_while = new DoWhile();
				token_stream.match(DO);
				do_while->blo = block();
				token_stream.match(WHILE);
				token_stream.match(LPAREN);
				do_while->condition = new Expr();
				token_stream.match(RPAREN);
				token_stream.match(SEMI);
				return do_while;
			}
			While *ret = new While();
			token_stream.match(WHILE);
			token_stream.match(LPAREN);
			ret->condition = new Expr();
			token_stream.match(RPAREN);
			ret->blo = block();
			return ret;
		}
		For * for_statement()
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
}