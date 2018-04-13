#pragma once
#include <vector>
#include "parser_node.hpp"
namespace Mer
{
	class Block:public ParserNode
	{
	public:
		std::vector<ParserNode*>ins_table;
		void new_block();
		void end_block();
		Mem::Object execute()override
		{
			for (const auto &a : ins_table)
				a->execute();
			return nullptr;
		}
	private:

	};

}