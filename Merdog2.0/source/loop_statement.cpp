#include "../include/loop_statement.hpp"
namespace Mer
{
	namespace Parser
	{
		While * while_statement()
		{
			While *ret = new While();
			token_stream.match(WHILE);
			token_stream.match(LPAREN);
			ret->condition = new Expr();
			token_stream.match(RPAREN);
			ret->blo = block();
			return ret;
		}
	}
}