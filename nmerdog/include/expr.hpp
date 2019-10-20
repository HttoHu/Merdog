#pragma once
#include "parser_node.hpp"
namespace mer
{
	class IntV :public ParserNode
	{
	public:
		IntV(int v);
	private:
		int value;
	};

}