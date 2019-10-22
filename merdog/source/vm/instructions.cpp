#include "../../include/vm/instructions.hpp"
#include "../../include/memory.hpp"
#include <stdio.h>
int mer::pc=0;
void mer::ins::push(const instruction_param& p)
{
	for (int i = 0; i < p.first; i++)
	{
		*memory[Reg[0]] = (char)(p.second >> (8 * i));
	}
}

void mer::ins::pop(const instruction_param& p)
{
	Reg[0] -= p.first;
}

void mer::ins::exit(const instruction_param& p)
{
	throw p.first;
}
