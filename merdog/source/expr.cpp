#include "../include/expr.hpp"
#include "../include/value.hpp"
#include "../include/memory.hpp"
using namespace Mer;
Mer::ParserNode * Mer::Expr::and_or()
{
	auto result = nexpr();
	while (token_stream.this_tag() == AND || token_stream.this_tag() == OR)
	{
		auto tok = token_stream.this_token();
		token_stream.next();
		std::cout << token_stream.this_token()->to_string();
		result = new BinOp(result, tok, nexpr());
	}
	return result;

}

Mer::ParserNode * Mer::Expr::expr()
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

Mer::ParserNode * Mer::Expr::nexpr()
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

Mer::ParserNode * Mer::Expr::term()
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

Mer::ParserNode * Mer::Expr::factor()
{
	auto result = token_stream.this_token();
	switch (result->get_tag())
	{
	case TRUE:
	{
		token_stream.match(TRUE);
		return new LConV(result);
	}
	case FALSE:
	{
		token_stream.match(FALSE);
		return new LConV(result);
	}
	case LPAREN:
	{
		token_stream.match(LPAREN);
		ParserNode* v = and_or();
		token_stream.match(RPAREN);
		return v;
	}
	case REAL:
	{
		token_stream.match(REAL);
		return new LConV(result);
	}
	case STRING:
	{
		token_stream.match(STRING);
		return new LConV(result);
	}
	case INTEGER:
	{
		token_stream.match(INTEGER);
		return new LConV(result);
	}
	case NOT:
	case MINUS:
	{
		token_stream.next();
		ParserNode *n = new UnaryOp(result, factor());
		return n;
	}
	case PLUS:
	{
		token_stream.match(PLUS);
		ParserNode* n = new UnaryOp(result, factor());
		return n;
	}
	case ID:
	{
		auto node = Parser::parse_id();
		return node;
	}
	default:
		return new NonOp();
	}
}

Mer::Mem::Object Mer::UnaryOp::execute()
{
	switch (op->get_tag())
	{
	case NOT:
	case MINUS:
	{
		auto tmp = expr->execute();
		auto ret = tmp->get_negation();
		return Mem::Object(ret);
	}
	case PLUS:
		return expr->execute();
	default:
		throw Error("no matched operator");
	}
}

Mem::Object Mer::Assign::execute()
{
	switch (asType)
	{
	case Mer::Assign::None:
		return stack_memory[left]->operator=(right->execute());
	case Mer::Assign::Add:
		return stack_memory[left]->operator+=(right->execute());
	case Mer::Assign::Sub:
		return stack_memory[left]->operator-=(right->execute());
	case Mer::Assign::Div:
		return stack_memory[left]->operator/=(right->execute());
	case Mer::Assign::Mul:
		return stack_memory[left]->operator*=(right->execute());
	default:
		throw Error("unkonwn assignment type");
	}
}
