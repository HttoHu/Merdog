/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/memory.hpp"
using namespace Mer;
Memory Mer::mem;
namespace Mer
{
	size_t Memory::new_block()
	{
		block_flag.push_back(index);
		return block_flag.back();
	}
	void Memory::new_func(size_t off)
	{
		// current is the last position of variable before called.
		current -= off;
		call_stack.push(off);
	}
	void Memory::end_func()
	{
		current +=call_stack.top();
		call_stack.pop();

	}
	size_t Memory::push(int size)
	{
		function_block_size += size;
		index += size;
		check();
		return index;
	}
	size_t Memory::push()
	{
		function_block_size++;
		check();
		return index++;
	}
	size_t Memory::end_block() {
		index = block_flag.back();
		block_flag.pop_back();
		return index;
	}
	Mem::Object& Memory::operator[](size_t in)
	{
		return stack_mem[in];
	}
	void Memory::reset()
	{
		glo_var_index = 0;
		current = 0;
		index = 0;
	}
	void Memory::alloc()
	{
		Mem::Object* tmp = new Mem::Object[capacity * 2];
		for (size_t i = 0; i < capacity; i++)
		{
			tmp[i] = stack_mem[i];
		}
		delete[] stack_mem;
		stack_mem = tmp;
	}
}