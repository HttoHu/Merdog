/*
	* Inspired by
	* https://ruslanspivak.com/lsbasi-part10/
	* Ruslan's Blog
	* C++ Version.
	* Yuantao Hu 2018
	* Email Huyuantao@outlook.com
*/
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
		For * for_statement()
		{
			For *ret = new For();
			token_stream.match(FOR);
			token_stream.match(LPAREN);
			ret->init = variable_declaration();
			token_stream.match(SEMI);
			ret->condition = new Expr();
			token_stream.match(SEMI);
			ret->step_action = statement();
			token_stream.match(RPAREN);
			ret->blo = block();
			return ret;
		}
	}
}