#include "../include/memory.hpp"
using namespace Mer;
Memory Mer::stack_memory;
Heap Mer::heap_memory;

size_t Mer::Memory::new_block() {
	//*(++block_flag2) = index;
	block_flag.push_back(index);
	return index;
}
size_t Mer::Memory::end_block()
{
	index = block_flag.back(); //*block_flag2;
	//block_flag2--;
	block_flag.pop_back();
	//return *block_flag2;
	return block_flag.back();
}

void Mer::Memory::new_func(int siz)
{
	current += siz;
	// if it is exploded ... e. Indeed I don't know how to do it. 
	// in particular, when you called a recursive function. Try a bigger size of call stack or delete some code, and reuse the passed code.
	// in an effort to a better perfomance, but not safe.  - 5% s.... 
	*(++call_stack2) = siz;
	//call_stack.push(siz);
}

void Mer::Memory::end_func()
{
	current -= *call_stack2;//call_stack.top();
	call_stack2--;
	//call_stack.pop();
}

size_t Mer::Memory::push()
{
	check();
	return index++;
}