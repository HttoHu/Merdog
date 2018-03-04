#include "../include/expr.hpp"
#include "../include/parser.hpp"
#include "../include/memory.hpp"
#include "../include/environment.hpp"
Mer::AST * Mer::Expr::and_or()
{
	auto result = nexpr();
	while (token_stream.this_tag() == AND || token_stream.this_tag() == OR)
	{
		auto tok = token_stream.this_token();
		token_stream.next();
		result = new BinOp(result, tok, nexpr());
	}
	return result;

}
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
		return new Unit(result);
	}
	case FALSE:
	{
		token_stream.match(FALSE);
		return new Unit(result);
	}
	case LPAREN:
	{
		token_stream.match(LPAREN);
		AST* v = and_or();
		token_stream.match(RPAREN);
		return v;
	}
	case REAL:
	{
		token_stream.match(REAL);
		return new Unit(result);
	}
	case STRING:
	{
		token_stream.match(STRING);
		return new Unit(result);
	}
	case INTEGER:
	{
		token_stream.match(INTEGER);
		return new Unit(result);
	}
	case GET_ADD:
	{
		token_stream.next();
		auto tok = token_stream.this_token();
		token_stream.match(ID);
		return new GetAdd(tok,symbol_table.find_type(tok));
	}
	case NOT:
	case MINUS:
	{
		token_stream.next();
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
		if (token_stream.next_token()->get_tag() == LPAREN)
			return Parser::func_call();
		 return Parser::variable();
	}
	default:
		return nullptr;
	}
}

Mer::Mem::Object Mer::UnaryOp::get_value()
{
	switch (op->get_tag())
	{
	case NOT:
	case MINUS:
	{
		auto tmp = expr->get_value();
		auto ret = tmp->get_negation();
		return Mem::Object(ret);
	}
	case PLUS:
		return expr->get_value();
	default:
		throw Error("no matched operator");
	}
}

Mer::GetAdd::GetAdd(Token * tok,size_t t):type(t)
{
	sz=find_pos(tok);
}

Mer::Mem::Object Mer::GetAdd::get_value()
{
	return std::make_shared<Mem::Ref>(_mem[sz]);
}

size_t Mer::GetAdd::get_type()
{
	return type;
}
