#pragma once
#include "lexer.hpp"
#include "type.hpp"
namespace Mer
{
	class AST
	{
	public:
		virtual ~AST() {}
		virtual Mem::Raw get_value() { return Mem::Raw(nullptr); }
	private:
	};
	class BinOp :public AST
	{
	public:
		BinOp(AST *l, Token *o, AST* r) :left(l), op(o), right(r) {}
		Mem::Raw get_value()override
		{
			auto left_v = left->get_value();
			auto right_v = right->get_value();
			Mem::Value* ret=nullptr;
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
			return Mem::Raw(ret);
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
		Mem::Raw get_value()override;
	private:
		Token *op;
		AST* expr;
	};
	class Num :public AST
	{
	public:
		Num(Token *t) :tok(t) {}
		Mem::Raw get_value()override
		{
			if (tok->get_tag() == INTEGER)
				return std::shared_ptr<Mem::Value>(new Mem::Int(Integer::get_value(tok)));
			else
				return std::shared_ptr<Mem::Value>(new Mem::Double(Real::get_value(tok)));
		}
	private:
		Token *tok;
	};
	class Expr
	{
	public:
		Expr() :tree(expr()){}
		Mem::Raw get_value()
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
