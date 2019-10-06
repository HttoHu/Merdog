#pragma once
#include <vector>
#include "parser_node.hpp"
// block is the set of santences. 
namespace Mer
{
	class Function;
	class Return;
	extern std::vector<ParserNode*> *current_ins_table;
	class Block:public ParserNode
	{
	public:
		friend Return;
		friend Function;
		std::vector<ParserNode*>ins_table;
		void new_block();
		void end_block();
		Mem::Object execute()override;
	private:
		//to record the pos of the instruction.
		int index = 0;
		// for function;
		Mem::Object ret=nullptr;
	};
}