#include "../include/expr.hpp"
#include "../include/parser.hpp"
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

Mer::AST * Mer::Expr::nexpr()
{
	auto result = expr();
	while (1)
	{
		auto tok = token_stream.this_token();
		switch (token_stream.this_token()->get_tag())
		{
		case EQ:
		case NE:
		case GE:
		case GT:
		case LE:
		case LT:
			token_stream.next();
			break;
		default:
			return result;
		}
		result = new BinOp(result, tok, expr());
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
	switch (result->get_tag())
	{
	case TRUE:
	{
		token_stream.match(TRUE);
		return new Num(result);
	}
	case FALSE:
	{
		token_stream.match(FALSE);
		return new Num(result);
	}
	case LPAREN:
	{
		token_stream.match(LPAREN);
		AST* v = expr();
		token_stream.match(RPAREN);
		return v;
	}
	case REAL:
	{
		token_stream.match(REAL);
		return new Num(result);
	}
	case STRING:
	{
		token_stream.match(STRING);
		return new Num(result);
	}
	case INTEGER:
	{
		token_stream.match(INTEGER);
		return new Num(result);
	}
	case MINUS:
	{
		token_stream.match(MINUS);
		AST *n = new UnaryOp(result, factor());
		return n;
	}
	case PLUS:
	{
		token_stream.match(PLUS);
		AST* n = new UnaryOp(result, factor());
		return n;
	}
	case ID:
	{
		auto node = Parser::variable();
		return node;
	}
	default:
		return nullptr;
	}
}

Mer::Mem::Object Mer::UnaryOp::get_value()
{
	if (op->get_tag() == MINUS)
	{
		auto tmp = expr->get_value();
		auto ret = tmp->get_negation();
		return Mem::Object(ret);
	}
	else
		return expr->get_value();
	//return Value();
}
