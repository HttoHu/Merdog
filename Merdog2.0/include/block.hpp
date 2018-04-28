#pragma once
#include <vector>
#include "parser_node.hpp"
namespace Mer
{
	class Block
	{
	public:
		std::vector<ParserNode*>ins_table;
		void run()
		{
			for (const auto &a : ins_table)
				a->execute();
		}
	private:

	};

}