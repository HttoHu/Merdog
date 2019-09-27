/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/expr.hpp"
#include "../include/value.hpp"
#include "../include/compound_box.hpp"
#include "../include/memory.hpp"
#include "../include/namespace.hpp"
#include "../include/function.hpp"
#include "../include/word_record.hpp"
using namespace Mer;
Mer::Expr::Expr(size_t t) :is_bool(false), expr_type(t) {
	tree = and_or();
	if (expr_type == 0 && tree->get_type() != 0)
	{
		expr_type = tree->get_type();
	}
}
size_t Mer::Expr::get_type()
{
	if (is_bool)
	{
		return Mem::BOOL;
	}
	if (tree == nullptr)
		return Mem::BVOID;
	if (expr_type != 0)
		return expr_type;
	return tree->get_type();
}
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
			throw Error("expr: syntax error");
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
		switch (tok->get_tag())
		{
		case EQ:
		case NE:
		case GE:
		case GT:
		case LE:
		case LT:
			is_bool = true;
		case ASSIGN:
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
			throw Error("tern: syntax error");
		result = new BinOp(result, tok, factor());
	}
	return result;
}

Mer::ParserNode * Mer::Expr::factor( )
{
	auto result = token_stream.this_token();
	switch (result->get_tag())
	{
	case DELETE:
		return new Delete();
	case MUL:
	{
		return new RmRef();
	}
	case GET_ADD:
		return new GetAdd();
	case BEGIN:
	{
		auto result = find_ustructure_t(expr_type);
		return new StructureInitList(result->mapping);
	}
	case CAST:
		return new Cast();
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
	case NEW:
		return new NewExpr();
	case ID:
	{
		return Parser::parse_id();
	}
	default:
		return new NonOp();
	}
}

Mer::BinOp::BinOp(ParserNode* l, Token* o, ParserNode* r):left(l), op(o), right(r)
{
	if (l->get_type() != r->get_type())
	{ 
		right = new Cast(r, l->get_type());
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

size_t Mer::BinOp::get_type()
{
	switch (op->get_tag())
	{
	case AND:
	case OR:
	case EQ:
	case NE:
	case GE:
	case GT:
	case LE:
	case LT:
		return Mem::BOOL;
	default:
		break;
	}
	return left->get_type();
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

Mer::Assign::Assign(AssignType a, ParserNode* l, Token* o, ParserNode* r) :asType(a), left(l), op(o), right(r) {
	if (l->get_type() != r->get_type())
		right =new Cast(r, l->get_type());
}

Mem::Object Mer::Assign::execute()
{
	switch (asType)
	{
	case Mer::Assign::None:
		return left->execute()->operator=(right->execute());
	case Mer::Assign::Add:
		return left->execute()->operator+=(right->execute());
	case Mer::Assign::Sub:
		return left->execute()->operator-=(right->execute());
	case Mer::Assign::Div:
		return left->execute()->operator/=(right->execute());
	case Mer::Assign::Mul:
		return left->execute()->operator*=(right->execute());
	default:
		throw Error("unkonwn assignment type");
	}
}

Mer::InitList::InitList(size_t t,size_t sz):type(t),size(sz)
{
	token_stream.match(BEGIN);
	while (token_stream.this_tag() != Tag::END)
	{
		init_v.push_back(new Expr(t));
		if (token_stream.this_tag() == Tag::COMMA)
			token_stream.match(COMMA);
	}
	token_stream.match(END);
	if (init_v.size() == 1 && sz > 1)
	{
		init_v = std::vector<Expr*>(sz, init_v[0]);
		return;
	}
	if (init_v.size() != sz)
		throw Error("Error, array overflow");

	for (int i = 1; i < init_v.size(); i++)
	{
		if (type != init_v[i - 1]->get_type())
		{ 
			throw Error("there is a type-distinction in an init list.");
		}
	}

}

Mem::Object Mer::InitList::execute()
{
	std::vector<Mem::Object> v(init_v.size());
	if (v.size() == 1 && size > 1)
	{
		auto tmp= init_v[0]->execute();
		v = std::vector<Mem::Object>(size, tmp);
		return std::make_shared<Mem::InitListObj>(std::move(v),type);
	}
	for (size_t i = 0; i < v.size(); i++)
	{
		v[i] = init_v[i]->execute();
	}
	return std::make_shared<Mem::InitListObj>(std::move(v),type);
}

std::vector<Mem::Object> Mer::InitList::get_array()
{
	std::vector<Mem::Object> v(init_v.size());
	if (v.size() == 1 && size > 1)
	{
		auto tmp = init_v[0]->execute();
		v = std::vector<Mem::Object>(size, tmp);
		return v;
	}
	if (v.size() != size)
		throw Error("list size is not matched with array");
	for (size_t i = 0; i < v.size(); i++)
	{
		v[i] = init_v[i]->execute();
	}
	return v;
}

Mer::InitList::~InitList()
{
	for (auto &a : init_v)
		delete a;
}

Mer::ImplicitConvertion::ImplicitConvertion(size_t _type) :type(_type) {}

Mem::Object Mer::ImplicitConvertion::execute()
{
	return tree->execute()->Convert(type);
}

Mem::Object Mer::ContainerIndex::execute()
{
	auto tmp = expr->execute();
	return mem[mem.get_current()+pos + std::static_pointer_cast<Mem::Int>(tmp)->get_value()];
}

size_t Mer::ContainerIndex::get_type()
{
	return type;
}

Mer::EmptyList::EmptyList(size_t t, size_t sz):type_code(t),size(sz)
{
	for (size_t i = 0; i < sz; i++)
	{
		init_v.push_back(new Expr(new LConV(Mem::create_var_t(t),t)));
	}
}

Mem::Object Mer::EmptyList::execute()
{
	return nullptr;
}

size_t Mer::ContainerGloIndex::get_pos()
{
	size_t off_set = Mem::get_raw<int>(expr->execute());
	return off_set + pos;
}

Mem::Object Mer::ContainerGloIndex::execute()
{
	auto tmp = expr->execute();
	return mem[pos + std::static_pointer_cast<Mem::Int>(tmp)->get_value()];
}

Mer::NewExpr::NewExpr()
{
	token_stream.match(NEW);
	size_t type_code = Mem::get_type_code();
	token_stream.match(LPAREN);
	expr = (new Expr(type_code))->tree;
	if (expr->get_type() != type_code)
	{
		throw Error("new-type not matched");
	}
	token_stream.match(RPAREN);
}

Mem::Object Mer::NewExpr::execute()
{
	auto pos = mem.new_obj();
	mem[pos] = expr->execute();
	return std::make_shared<Mem::Int>(pos);
}

Mer::GetAdd::GetAdd()
{
	token_stream.match(GET_ADD);
	id = new Expr();
	type = id->get_type();
}

size_t Mer::GetAdd::get_type()
{
	return type+1;
}

Mem::Object Mer::GetAdd::execute()
{
	return std::make_shared<Mem::Pointer>(id->get_pos());
}

Mer::RmRef::RmRef()
{
	token_stream.match(MUL);
	auto name = Id::get_value(token_stream.this_token());
	auto result=this_namespace->sl_table->find(name);
	if (result == nullptr)
		throw Error("Var " + name + "no found");
	id = Parser::parse_var(result);
	type = id->get_type();
}

size_t Mer::RmRef::get_type()
{
	return type-1;
}
Mem::Object Mer::RmRef::execute()
{
	if (mem[1] == nullptr)
		throw Error("WHAT");
	return mem[Mem::get_raw<int>(id->execute())];
}

Mer::Delete::Delete()
{
	token_stream.match(DELETE);
	expr =(new Expr())->root();
}

Mem::Object Mer::Delete::execute()
{
	mem.del_obj(Mem::get_raw<int>(expr->execute()));
	return nullptr;
}
