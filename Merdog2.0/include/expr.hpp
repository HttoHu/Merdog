#pragma once
#include "parser_node.hpp"
#include "lexer.hpp"
namespace Mer
{
	class BinOp :public ParserNode
	{
	public:
		BinOp(ParserNode *l, Token *o, ParserNode* r) :left(l), op(o), right(r) 
		{

		}
		Mem::Object execute()override
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
				ret = left_v->operator<=(right_v);
				break;
			case LE:
				ret = left_v->operator<(right_v);
				break;
			default:
				break;
			}
			return Mem::Object(ret);
		}
	private:
		ParserNode *left;
		Token *op;
		ParserNode *right;
	};
	class UnaryOp :public ParserNode
	{
	public:
		UnaryOp(Token *t, ParserNode* e) :op(t), expr(e) {}
		Mem::Object execute()override;
	private:
		Token *op;
		ParserNode* expr;
	};
	class Expr:public ParserNode
	{
	public:
		Expr() :tree(and_or()) {}
		Mem::Object execute()override
		{
			return tree->execute();
		}
		ParserNode *root() { return tree; }
	private:
		ParserNode *and_or();
		ParserNode *expr();
		ParserNode *nexpr();
		ParserNode *term();
		ParserNode *factor();
		ParserNode *tree;
	};
	class Assign :public ParserNode
	{
	public:
		enum AssignType
		{
			None,
			Add,Sub,Div,Mul,
		};
		Assign(AssignType a,size_t l, Token* o, ParserNode* r) :asType(a),left(l), op(o), right(r) {}
		Mem::Object execute()override;
	private:
		AssignType asType;
		size_t left;
		Token *op;
		ParserNode* right;
	};
}