#pragma once
#include "parser_node.hpp"
namespace mer
{
	// immediate number , 1,2,56778, and so on ....
	class IntV final:public ParserNode
	{
	public:
		IntV(int v) :value(v), ParserNode(ParserNode::CINTV) {}
		std::string get_gen()override;
	private:
		int value;
	};
	class BinOp final :public ParserNode
	{
	public:
	private:
		Token* op;
	};
}