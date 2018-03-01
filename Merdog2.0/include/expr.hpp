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
	private:
	};
	class BinOp :public AST
	{
	public:
		BinOp(AST *l, Token *o, AST* r) :left(l), op(o), right(r) {}
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
	private:
		Token *op;
		AST* expr;
	};
	class Unit :public AST
	{
	public:
		Unit(Token *t) :tok(t) {}
		Mem::Object get_value()override
		{
			switch (tok->get_tag())
			{
			case TRUE:
				return std::make_shared<Mem::Bool>(true);
			case FALSE:
				return std::make_shared<Mem::Bool>(false);
			case INTEGER:
				return std::make_shared<Mem::Int>(Integer::get_value(tok));
			case REAL:
				return std::make_shared<Mem::Double >(Real::get_value(tok));
			case STRING:
				return std::make_shared<Mem::String>(String::get_value(tok));
			default:
				throw Error("syntax error");
			}
		}
		virtual ~Unit(){}
	private:
		Token *tok;
	};
	class Expr
	{
	public:
		Expr() :tree(and_or()) {}
		Mem::Object get_value()
		{
			return tree->get_value();
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
