/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/block.hpp"
#include "../include/memory.hpp"
std::vector<Mer::ParserNode*> *Mer::current_ins_table=nullptr;
void Mer::Block::new_block()
{
	mem.new_block();
}

void Mer::Block::end_block()
{
	mem.end_block();
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
