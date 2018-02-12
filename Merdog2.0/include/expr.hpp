#pragma once
#include "lexer.hpp"
namespace Mer
{
	using Value = int;
	class AST
	{
	public:
		virtual ~AST() {}
		virtual Value get_value() { return 0; }
	private:
	};
	class BinOp :public AST
	{
	public:
		BinOp(AST *l, Token *o, AST* r) :left(l), op(o), right(r) {}
		Value get_value()override
		{
			switch (op->get_tag())
			{
			case PLUS:
				return left->get_value() + right->get_value();
			case MINUS:
				return left->get_value() - right->get_value();
			case MUL:
				return left->get_value()*right->get_value();
			case DIV:
				return left->get_value() / right->get_value();
			default:
				break;
			}
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
		Value get_value()override;
	private:
		Token *op;
		AST* expr;
	};
	class Num :public AST
	{
	public:
		Num(Token *t) :tok(t) {}
		Value get_value()override
		{
			return Integer::get_value(tok);
		}
	private:
		Token *tok;
	};
	class Expr
	{
	public:
		Expr() :tree(expr()){}
		int get_value()
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
