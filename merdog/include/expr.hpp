#pragma once
#include "parser_node.hpp"
#include "id_and_value.hpp"
namespace mer
{
	// immediate number , 1,2,56778, and so on ....

	//NonOp does nothing...
	class NonOp final :public ParserNode
	{
	public:
		NonOp() :ParserNode(NONOP) {}
	private:
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

	using BinOpNode=std::shared_ptr<IntV>;
	namespace analyse_expr
	{
		Node create_expr();
		Node assign();
		Node expr();
		Node term();
		Node factor();
	}
}