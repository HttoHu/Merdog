#pragma once
#include <iostream>
#include <set>
#include <tuple>
#include <vector>
namespace mer
{
	struct instruction_param
	{
		int first;
		int second;
		int dest;
	};
	using instruction = void(*)(const instruction_param & p);
	// rsp <=Reg[0]
	// rax <=Reg[1]
	std::set<int> residual_reg;
	extern int64_t Reg[16];
	void init_merdog();
	void run();
	extern int pc;
	extern std::vector<std::pair<instruction,instruction_param >> ins_table;
}