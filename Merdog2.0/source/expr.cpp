#include "../include/expr.hpp"

Mer::AST * Mer::Expr::expr()
{
	auto result = term();
	while (token_stream.this_token()->get_tag() == PLUS || token_stream.this_token()->get_tag() == MINUS)
	{
		auto tok = token_stream.this_token();
		if (token_stream.this_token()->get_tag() == PLUS)
			token_stream.match(PLUS);
		else if (token_stream.this_token()->get_tag() == MINUS)
			token_stream.match(MINUS);
		else
			throw Error("syntax error");
		result = new BinOp(result, tok, term());
	}
	return result;
}

Mer::AST * Mer::Expr::term()
{
	auto result = factor();
	while (token_stream.this_token()->get_tag() == MUL || token_stream.this_token()->get_tag() == DIV)
	{
		auto tok = token_stream.this_token();
		if (token_stream.this_token()->get_tag() == MUL)
			token_stream.match(MUL);
		else if (token_stream.this_token()->get_tag() == DIV)
			token_stream.match(DIV);
		else
			throw Error("syntax error");
		result = new BinOp(result, tok, factor());
	}
	return result;
}

Mer::AST * Mer::Expr::factor()
{
	auto result = token_stream.this_token();
	if (result->get_tag() == LPAREN)
	{
		token_stream.match(LPAREN);
		AST* v = expr();
		token_stream.match(RPAREN);
		return v;
	}
	token_stream.match(INTEGER);
	return new Num(result);
}