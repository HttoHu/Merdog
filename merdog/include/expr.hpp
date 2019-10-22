#pragma once
#include "parser_node.hpp"
namespace mer
{
	// immediate number , 1,2,56778, and so on ....
	class IntV final:public ParserNode
	{
	public:
		IntV(int v);
		IntV();
		std::string to_string()override;
	private:
		int value;
	};
	// BinOp receive two arguments , for instance: a+b, a is the left v, b is the right v
	// '+' is op, BinOp generate gen or execute directly.
	class BinOp final :public ParserNode
	{
	public:
		BinOp(Node l, Token* _op, Node r) :ParserNode(BINOP),left(l), op(_op), right(r) 
		{
			set_type(left->get_type());
			if (get_type() == nullptr)
				throw Error("我日了狗了1");
		}
		std::string get_gen()override;
		std::string to_string()override
		{
			return var_name;
		}
	private:
		std::string var_name;
		Token* op;
		Node left;
		Node right;
	};
	using IntVNode=std::shared_ptr<IntV>;
	using BinOpNode=std::shared_ptr<IntV>;
	namespace analyse_expr
	{
		Node expr();
		Node term();
		Node factor();
	}
}