#include "../HPP/TMemory.hpp"
using namespace Simcc;
void Simcc::StackBlock::
check()
{
	if (pc >= length / 2)
	{
		char* newMem = new char[length * 2];
		for (int i = 0; i < pc; i++)
			newMem[i] = mem[i];
		delete[] mem;
		mem = newMem;
		length *= 2;
	}
}

void Simcc::StackBlock::
new_block()
{
	pos_stack[pos_stack_index++] = pc;
}

void Simcc::StackBlock::
end_block()
{
	pc = pos_stack[pos_stack_index--];
}

char * Simcc::StackBlock::
find(size_t pos)
{
	return mem + pos;
}

void Simcc::StackBlock::
push_temp(const char *c_str)
{
	std::string str = c_str;
	char* d = (char*)&str;
	for (int i = 0; i <sizeof(std::string); i++)
		mem[tp_pos++] = d[i];
}

void Simcc::StackBlock::
push(const char *c_str)
{
	std::string str = c_str;
	check();
	char* d = (char*)&str;
	for (int i = 0; i < sizeof(str); i++)
		mem[pc++] = d[i];
}
char * StackBlock::mem = new char[1024];
int StackBlock::length = 1024;
unsigned char  StackBlock::tp_pos = 0;
int  StackBlock::pos_stack[20];
int  StackBlock::pos_stack_index = 0;
int StackBlock::pc = 0;