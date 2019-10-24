#pragma once
#include <iostream>
#include <set>
#include <tuple>
#include <vector>
#include <bitset>
#define RSP 0
#define RAX 1
#define RBX 2
namespace mer
{
	struct instruction_param
	{
		int64_t first;
		int64_t  second;
		int64_t  dest;
	};
	using instruction = void(*)(const instruction_param & p);
	// rsp <=Reg[0]
	// rax <=Reg[1]
	// rbx <=Reg[2]
	extern std::bitset<16> residual_register;
	extern int64_t Reg[16];
	extern std::vector<std::string> ir_instructions;
	void init_merdog();
	void run();
	extern int pc;
	extern std::vector<std::pair<instruction,instruction_param >> ins_table;
}