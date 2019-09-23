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
	capacity *= 2;
}
