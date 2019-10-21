#pragma once
#include "parser_node.hpp"
namespace mer
{
	// immediate number , 1,2,56778, and so on ....
	class IntV final:public ParserNode
	{
	public:
		IntV(int v);
		std::string get_gen()override;
	private:
		int value;
	};
	class BinOp final :public ParserNode
	{
	public:
		BinOp(ParserNode* l, Token* _op, ParserNode* r) :ParserNode(BINOP),left(l), op(_op), right(r) {}
		std::string get_gen()override;
	private:
		Token* op;
		ParserNode* left;
		ParserNode* right;
	};
	namespace analyse_expr
	{

	}
}