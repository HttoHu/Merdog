#pragma once
#include <iostream>
#include <tuple>
#include <vector>
namespace mer
{
	struct instruction_param
	{
		size_t first;
		size_t second;
		size_t dest;
	};
	using instruction = void(*)(const instruction_param & p);
	namespace reg
	{
		extern int64_t rax;
		extern int64_t rbx;
		extern int64_t rcx;
		extern int64_t rdx;
		extern int64_t rsi;
		extern int64_t rdi;
		extern int64_t rbp;
		extern int64_t rsp;
		extern int64_t r8;
		extern int64_t r9;
		extern int64_t r10;
		extern int64_t r11;
		extern int64_t r12;
		extern int64_t r13;
		extern int64_t r14;
		extern int64_t r15;
	}

	extern size_t pc;
	extern std::vector<std::pair<instruction,instruction_param >> ins_table;
	void run();
}