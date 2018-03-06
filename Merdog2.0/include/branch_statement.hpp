/*
*		Inspired by
*		https://ruslanspivak.com/lsbasi-part10/
*		Ruslan's Blog
*		C++ Version.
*		Yuantao Hu 2018
*		Email :Huyuantao@outlook.com
*/
#pragma once
#include "parser.hpp"
namespace Mer
{
	class If :public AST
	{
	public:
		Mem::Object get_value();
		std::vector<std::pair<Expr*, Block*>> if_block;
		Block* else_block;
	};
	namespace Parser
	{
		If *if_statement();
	}
}