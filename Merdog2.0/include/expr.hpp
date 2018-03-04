#pragma once
#include "lexer.hpp"
#include "type.hpp"
namespace Mer
{
	class CallFunc;
	class AST
	{
	public:
		virtual ~AST() {}
		virtual Mem::Object get_value() { return Mem::Object(nullptr); }
		virtual size_t get_type() { return 0; }
	};
	class BinOp :public AST
	{
	public:
		BinOp(AST *l, Token *o, AST* r) :left(l), op(o), right(r) 
		{
			if (left->get_type() != right->get_type())
				throw Error("type not matched");
		}
		Mem::Object get_value()override
		{
			auto left_v = left->get_value();
			auto right_v = right->get_value();
			Mem::Object ret = nullptr;
			switch (op->get_tag())
			{
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
				ret = left_v->operator<=(right_v);
				break;
			case LE:
				ret = left_v->operator<(right_v);
				break;
			default:
				throw Error("unknow operator");
				break;
			}
			return Mem::Object(ret);
		}
		size_t get_type()override {
			return left->get_type();
		}
	private:
		AST *left;
		Token *op;
		AST *right;
	};
	class UnaryOp :public AST
	{
	public:
		UnaryOp(Token *t, AST* e) :op(t), expr(e) {}
		Mem::Object get_value()override;
		size_t get_type()override
		{
			return expr->get_type();
		}
	private:
		Token *op;
		AST* expr;
	};
	class GetAdd :public AST
	{
	public:
		GetAdd(Token *tok,size_t t);
		Mem::Object get_value()override;
		size_t get_type()override;
	private:
		size_t type;
		size_t sz;
	};
	class Unit :public AST
	{
	public:
		Unit(Token *t) 
		{
			switch (t->get_tag())
			{
			case TRUE:
				value= std::make_shared<Mem::Bool>(true);
				break;
			case FALSE:
				value= std::make_shared<Mem::Bool>(false);
				break;
			case INTEGER:
				value= std::make_shared<Mem::Int>(Integer::get_value(t));
				break;
			case REAL:
				value= std::make_shared<Mem::Double >(Real::get_value(t));
				break;
			case STRING:
				value= std::make_shared<Mem::String>(String::get_value(t));
				break;
			default:
				throw Error("type error");
			}
		}
		Mem::Object get_value()override
		{
			return value;
		}
		size_t get_type()override {
			return value->get_type_code();
		}
		virtual ~Unit(){}
	private:
		Mem::Object value;
	};
	class Expr:public AST
	{
	public:
		Expr() :tree(and_or()) {}
		Mem::Object get_value()override
		{
			return tree->get_value();
		}
		size_t get_type()override
		{
			return tree->get_type();
		}
		AST *root() { return tree; }
	private:
		AST *and_or();
		AST* expr();
		AST* nexpr();
		AST* term();
		AST *factor();
		AST *tree;
	};
}
/*

*/
