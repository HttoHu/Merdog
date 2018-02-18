#include "../include/branch_statement.hpp"
namespace Mer
{
	namespace Parser
	{
		If * if_statement()
		{
			If *ret = new If();
			token_stream.match(IF);
			token_stream.match(LPAREN);
			auto expr = new Expr();
			token_stream.match(RPAREN);
			auto blo = Parser::block();
			ret->if_block.push_back({ expr,blo });
			while (token_stream.this_tag() == ELSE_IF)
			{
				token_stream.match(ELSE_IF);
				token_stream.match(LPAREN);
				auto expr = new Expr();
				token_stream.match(RPAREN);
				auto blo = Parser::block();
				ret->if_block.push_back({ expr,blo });
			}
			if (token_stream.this_tag() == ELSE)
			{
				token_stream.match(ELSE);
				ret->else_block = Parser::block();
			}
			return ret;
		}
	}
}