#include "../include/memory.hpp"
using namespace Mer;
Memory Mer::stack_memory;
Heap Mer::heap_memory;
void Memory::alloc()
{
	capacity *= 2;
	Mem::Object *tmp = new Mem::Object[capacity];
	for (size_t i = 0; i < current; i++)
	{
		tmp[i] = _mem[i];
	}
	delete[] _mem;
	_mem = tmp;
}