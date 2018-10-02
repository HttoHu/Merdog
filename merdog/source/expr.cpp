#include "../include/expr.hpp"
#include "../include/value.hpp"
#include "../include/memory.hpp"
#include "../include/structure.hpp"
#include "../include/function.hpp"
using namespace Mer;
Mer::ParserNode * Mer::Expr::and_or()
{
	auto result = nexpr();
	while (token_stream.this_tag() == AND || token_stream.this_tag() == OR)
	{
		is_bool = true;
		auto tok = token_stream.this_token();
		token_stream.next();
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
	case TTRUE:
	{
		token_stream.match(TTRUE);
		return new LConV(result);
	}
	case TFALSE:
	{
		token_stream.match(TFALSE);
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
		ParserNode* id = Parser::parse_id();
		if (token_stream.this_tag() == LSB)
		{
			auto tmp = token_stream.this_token();
			token_stream.match(LSB);

			auto ret = new BinOp(id, tmp, new Expr());
			token_stream.match(RSB);
			return ret;
		}
		return id;
	}
	default:
		return new NonOp();
	}
}

Mem::Object BinOp::execute()
{
	auto left_v = left->execute();
	auto right_v = right->execute();
	Mem::Object ret = nullptr;
	switch (op->get_tag())
	{
	case SADD:
		ret = left_v->operator+=(right_v);
		break;
	case SSUB:
		ret = left_v->operator-=(right_v);
		break;
	case SDIV:
		ret = left_v->operator/=(right_v);
		break;
	case SMUL:
		ret = left_v->operator*=(right_v);
		break;
	case ASSIGN:
		ret = left_v->operator=(right_v);
		break;
	case AND:
		ret = left_v->operator&&(right_v);
		break;
	case OR:
		ret = left_v->operator||(right_v);
		break;
	case PLUS:
		ret = left_v->operator+(right_v);
		break;
	case MINUS:
		ret = left_v->operator-(right_v);
		break;
	case MUL:
		ret = left_v->operator*(right_v);
		break;
	case DIV:
		ret = left_v->operator/(right_v);
		break;
	case EQ:
		ret = left_v->operator==(right_v);
		break;
	case NE:
		ret = left_v->operator!=(right_v);
		break;
	case GT:
		ret = left_v->operator>(right_v);
		break;
	case GE:
		ret = left_v->operator>=(right_v);
		break;
	case LT:
		ret = left_v->operator<(right_v);
		break;
	case LE:
		ret = left_v->operator<=(right_v);
		break;
	case LSB:
		ret = left_v->operator[](right_v);
		return ret;
	default:
		break;
	}
	return Mem::Object(ret);
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
/*
Mer::InitList::InitList(Structure * _type, const std::map<std::string, Expr*>& _init_list) {
	type = _type->type_id();
	init_v.resize(_type->type_size());
	for (const auto &a : _init_list)
	{
		init_v[_type->find_var(a.first).first] = a.second;
	}
}

Mem::Object Mer::InitList::execute()
{
	std::vector<Mem::Object> v(init_v.size());
	for (size_t i = 0; i < v.size(); i++)
	{
		v[i] = init_v[i]->execute();
	}
	return std::make_shared<CompoundObject>(type,v);
}

Mer::InitList::~InitList()
{
	for (auto &a : init_v)
		delete a;
}
*/

Mer::ImplicitConvertion::ImplicitConvertion(size_t _type) :type(_type) {}

Mem::Object Mer::ImplicitConvertion::execute()
{
	return tree->execute()->Convert(type);
}
