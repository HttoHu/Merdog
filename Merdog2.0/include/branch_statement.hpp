#pragma once
#include "parser.hpp"
namespace Mer
{
	class If :public AST
	{
	public:
		Mem::Object get_value()
		{
			for (auto &a : if_block)
			{
				if (std::static_pointer_cast<Mem::Bool>(a.first->get_value())->_value())
				{
					a.second->get_value();
					return nullptr;
				}
			}
			if (else_block != nullptr)
				else_block->get_value();
			return nullptr;
		}
		std::vector<std::pair<Expr*, Block*>> if_block;
		Block* else_block;
	};
	namespace Parser
	{
		If *if_statement();
	}
}