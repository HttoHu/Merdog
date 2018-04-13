#include "../include/loop_statement.hpp"
namespace Mer
{
	namespace Parser
	{
		Mer::While * while_statement()
		{
			Mer::While *ret = new While();
			token_stream.match(WHILE);
			token_stream.match(LPAREN);
			ret->condition = new Expr();
			token_stream.match(RPAREN);
			ret->blo = block();
			return ret;
		}
		For * for_statement()
		{
			For *ret = new For();
			token_stream.match(FOR);
			token_stream.match(LPAREN);
			ret->init = var_decl();

			token_stream.match(SEMI);

			ret->condition = new Expr();
			token_stream.match(SEMI);
	
			ret->step_action = assignment_statement();
			token_stream.match(RPAREN);
			ret->blo = block();
			return ret;
		}
	}
}