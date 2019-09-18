/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/memory.hpp"
using namespace Mer;
Memory Mer::mem;
Heap Mer::heap_memory;
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
size_t Mer::Memory::push_to_static()
{
	check_static();
	return sindex++;
}
size_t Mer::Memory::push_to_static(size_t sz)
{
	sindex += sz;
	this->check_static();
	return sindex;
}
size_t Mer::Memory::end_block() {
	index = block_flag.back();
	block_flag.pop_back();
	return index;
}
Mem::Object& Mer::Memory::static_index(size_t in)
{
	return static_mem[in];
}
Mem::Object& Mer::Memory::operator[](size_t in)
{
	return stack_mem[in + current];
}
void Mer::Memory::check_static()
{
	if (sindex > 0.5L * scapacity)
		salloc();
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

void Mer::Memory::salloc()
{
	scapacity *= 2;
	Mem::Object* tmp = new Mem::Object[scapacity];
	for (size_t i = 0; i < scapacity/2; i++)
	{
		tmp[i] = static_mem[i];
	}
	delete[] static_mem;
	static_mem = tmp;
}
