/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/memory.hpp"
using namespace Mer;
Memory Mer::mem;
namespace Mer
{
	void Memory::new_func(int siz)
	{
		current += siz;
		call_stack.push(siz);
	}
	void Memory::end_func()
	{
		current -= call_stack.top();

		call_stack.pop();
	}
	size_t Memory::push(int size)
	{
		index += size;
		function_size += size;
		check();
		return index;
	}
	size_t Memory::push()
	{
		check();
		function_size++;
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
	void Memory::resert_function_size()
	{
		function_size = 0;
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