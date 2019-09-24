/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/memory.hpp"
using namespace Mer;
Memory Mer::mem;
size_t Mer::Memory::push(int size)
{
	index += size;
	check();
	return index;
}
size_t Mer::Memory::push()
{
	check();
	return index++;
}
size_t Mer::Memory::end_block() {
	index = block_flag.back();
	block_flag.pop_back();
	return index;
}
size_t Mer::Memory::new_obj()
{
	if (free_pos_stack.empty())
		return heap_pos + (heap_index++);
	auto ret= free_pos_stack.top();
	free_pos_stack.pop();
	return ret;
}
void Mer::Memory::del_obj(size_t sz)
{
	free_pos_stack.push(sz);
}
Mem::Object& Mer::Memory::operator[](size_t in)
{
	return stack_mem[in + current];
}
void Memory::alloc()
{
	Mem::Object *tmp = new Mem::Object[capacity*2];
	for (size_t i = 0; i < capacity; i++)
	{
		tmp[i] = stack_mem[i];
	}
	delete[] stack_mem;
	stack_mem = tmp;
}
