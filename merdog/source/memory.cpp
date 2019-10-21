#include "../include/memory.hpp"

mer::Memory::Memory()
{
	mem = new char[MERDOG_MEM_SIZE];
}
namespace mer
{
	Memory memory;
}