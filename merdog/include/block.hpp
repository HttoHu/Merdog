#pragma once
#include <vector>
#include "parser_node.hpp"
namespace Mer
{
	class Return;
	class Block:public ParserNode
	{
	public:
		friend Return;
		std::vector<ParserNode*>ins_table;
		void new_block();
		void end_block();
		Mem::Object execute()override;
	private:
		int index = 0;
		Mem::Object ret=nullptr;
	};

}