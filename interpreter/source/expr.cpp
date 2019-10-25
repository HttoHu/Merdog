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
	tree = assign();
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
ParserNode* Mer::Expr::assign()
{
	auto result = and_or();
	while (token_stream.this_tag() == ASSIGN || token_stream.this_tag() == SADD || token_stream.this_tag() == SSUB || token_stream.this_tag() == SMUL || token_stream.this_tag() == SDIV)
	{
		auto tok = token_stream.this_token();
		token_stream.next();
		result = new BinOp(result, tok, and_or());
	}
	return result;
}
Mer::ParserNode* Mer::Expr::and_or()
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

Mer::ParserNode* Mer::Expr::expr()
{
	auto result = term();

	while (token_stream.this_token()->get_tag() == PLUS || token_stream.this_token()->get_tag() == MINUS)
	{
		auto tok = token_stream.this_token();
		token_stream.next();
		result = new BinOp(result, tok, term());
	}
	return result;
}

Mer::ParserNode* Mer::Expr::nexpr()
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
			token_stream.next();
			break;
		default:
			return result;
		}
		result = new BinOp(result, tok, expr());
	}
	return result;
}

Mer::ParserNode* Mer::Expr::term()
{
	auto result = member_visit();
	while (token_stream.this_token()->get_tag() == MUL || token_stream.this_token()->get_tag() == DIV)
	{
		auto tok = token_stream.this_token();
		token_stream.next();
		result = new BinOp(result, tok, member_visit());
	}
	return result;
}

ParserNode* Mer::Expr::member_visit()
{
	auto result = factor();
	while (token_stream.this_token()->get_tag() == DOT || token_stream.this_token()->get_tag() == PTRVISIT)
	{
		auto tok = token_stream.this_token();
		token_stream.next();
		auto member_id = token_stream.this_token();
		token_stream.match(ID);
		size_t type_code = result->get_type();
		// member function

		// find struct info

		auto ustruct = find_ustructure_t(type_code + (type_code % 2 - 1));
		if (token_stream.this_tag() == LPAREN)
		{
			// get type code
			auto func = ustruct->member_function_table.find(Id::get_value(member_id));
			if (func == ustruct->member_function_table.end())
				throw Error(" member functioon " + Id::get_value(member_id) + " no found");
			result = Parser::parse_call_by_function(func->second, new GetAdd(result));
			continue;

		}
		// find member index and type;
		auto seeker = ustruct->get_member_info(Id::get_value(member_id));
		result = new Index(result, seeker.second, seeker.first);
	}
	return result;
}
Mer::ParserNode* Mer::Expr::factor()
{
	auto result = token_stream.this_token();
	switch (result->get_tag())
	{
	case NEW:
		return new NewExpr();
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
		ParserNode* n = new UnaryOp(result, factor());
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
		return Parser::parse_id();
	}
	case NULLPTR:
		token_stream.match(NULLPTR);
		return new LConV(std::make_shared<Mem::Pointer>(nullptr), expr_type);
	default:
		return new NonOp();
	}
}

Mer::BinOp::BinOp(ParserNode* l, Token* o, ParserNode* r) :left(l), op(o), right(r)
{
	if (o->get_tag() != LSB && l->get_type() != r->get_type())
	{
		if (!(l->get_type() % 2 && r->get_type() % 2))
			return;
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
		throw Error("Undefined operator");
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

std::string Mer::BinOp::to_string()
{
	return left->to_string() + op->to_string() + right->to_string();
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


Mer::InitList::InitList(size_t t, size_t sz) :type(t), size(sz)
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
		auto tmp = init_v[0]->execute();
		v = std::vector<Mem::Object>(size, tmp);
		return std::make_shared<Mem::InitListObj>(std::move(v), type);
	}
	for (size_t i = 0; i < v.size(); i++)
	{
		v[i] = init_v[i]->execute();
	}
	return std::make_shared<Mem::InitListObj>(std::move(v), type);
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
	for (auto& a : init_v)
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
	return mem[mem.get_current() + pos + std::static_pointer_cast<Mem::Int>(tmp)->get_value()];
}

size_t Mer::ContainerIndex::get_type()
{
	return type;
}

Mer::EmptyList::EmptyList(size_t t, size_t sz) :type_code(t), size(sz)
{
	for (size_t i = 0; i < sz; i++)
	{
		init_v.push_back(new Expr(new LConV(Mem::create_var_t(t), t)));
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
	if (token_stream.this_tag() == LPAREN)
	{
		expr = (new Expr(type_code))->tree;
	}
	else if (token_stream.this_tag() == BEGIN)
	{
		auto _result = find_ustructure_t(type_code);
		expr = new StructureInitList(_result->mapping, type_code);
	}
	else if (token_stream.this_tag() == SEMI)
	{
		auto insertion = Mem::create_var_t(type_code);
		expr = new LConV(insertion, type_code);
	}
	if (expr->get_type() != type_code)
	{
		throw Error("new-type not matched from " + type_to_string(expr->get_type()) + " to " + type_to_string(type_code));
	}
}

Mem::Object Mer::NewExpr::execute()
{
	return std::make_shared<Mem::Pointer>(expr->execute()->clone());
}

Mer::GetAdd::GetAdd()
{
	token_stream.match(GET_ADD);
	id = Expr().root();
	type = id->get_type();
}

size_t Mer::GetAdd::get_type()
{
	return type + 1;
}

Mem::Object Mer::GetAdd::execute()
{
	return std::make_shared<Mem::Pointer>(id->execute());
}

Mer::RmRef::RmRef()
{
	token_stream.match(MUL);
	auto name = Id::get_value(token_stream.this_token());
	auto result = this_namespace->sl_table->find(name);
	if (result == nullptr)
		throw Error("Var " + name + " no found");
	id = Parser::parse_var(result);
	type = id->get_type();
}

size_t Mer::RmRef::get_type()
{
	return type - 1;
}
Mem::Object Mer::RmRef::execute()
{
	
	return std::static_pointer_cast<Mem::Pointer>(id->execute())->rm_ref();
}

Mer::Delete::Delete()
{
	token_stream.match(DELETE);
	expr = Expr().root();
}

Mem::Object Mer::Delete::execute()
{
	return nullptr;
}

Mer::Index::Index(ParserNode* l, size_t _index, size_t _type) :left(l), index(_index), type(_type)
{
	if (_type == -1)
		type = left->get_type();
}
Mem::Object Mer::Index::execute()
{
	return left->execute()->operator[](std::make_shared<Mem::Int>(index));
}

size_t Mer::Index::get_type()
{
	return type;
}