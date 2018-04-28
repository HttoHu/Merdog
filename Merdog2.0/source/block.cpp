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
