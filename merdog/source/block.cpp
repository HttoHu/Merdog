#include "../include/block.hpp"
#include "../include/memory.hpp"
void Mer::Block::new_block()
{
	stack_memory.new_block();
}

void Mer::Block::end_block()
{
	stack_memory.end_block();
}

Mer::Mem::Object Mer::Block::execute()
{
	ret = nullptr;
	size_t tmp = index;
	for (index=0; index < (ins_table.size()); index++)
	{
		ins_table[index]->execute();
	}
	index = tmp;
	return ret;
}
