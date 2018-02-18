#pragma once
#include "lexer.hpp"
#include "type.hpp"
namespace Mer
{
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
			Mem::Value* ret = nullptr;
			switch (op->get_tag())
			{
			case PLUS:
			{
				ret = left_v->operator+(&(*right_v));
				break;
			}
			case MINUS:
			{
				ret = left_v->operator-(&(*right_v));
				break;
			}
			case MUL:
			{
				ret = left_v->operator*(&(*right_v));
				break;
			}
			case DIV:
			{
				ret = left_v->operator/(&(*right_v));
				break;
			}
			default:
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
	class Num :public AST
	{
	public:
		Num(Token *t) :tok(t) {}
		Mem::Object get_value()override
		{
			switch (tok->get_tag())
			{
			case INTEGER:
				return std::make_shared<Mem::Int>(Integer::get_value(tok));
			case REAL:
				return std::make_shared < Mem::Double >(Real::get_value(tok));
			case STRING:
				return std::make_shared<Mem::String>(String::get_value(tok));
			default:
				throw Error("syntax error");
			}
		}
	private:
		Token *tok;
	};
	class Expr
	{
	public:
		Expr() :tree(expr()) {}
		Mem::Object get_value()
		{
			return tree->get_value();
		}
		AST *root() { return tree; }
	private:
		AST* expr();
		AST* term();
		AST *factor();
		AST *tree;
	};
}
/*

*/
