#include "../include/instructions.hpp"
#include "../include/memory.hpp"
#include <stdio.h>
void mer::ins::push(const instruction_param& p)
{
	for (size_t i = 0; i < p.first; i++)
	{
		*memory[reg::rsp++] = (char)(p.second >> (8 * i));
	}
}

void mer::ins::pop(const instruction_param& p)
{
	reg::rsp -= p.first;
}

void mer::ins::exit(const instruction_param& p)
{
	throw p.first;
}
