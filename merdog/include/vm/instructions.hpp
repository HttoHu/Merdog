#pragma once
#include "../environment.hpp"
#include "../memory.hpp"
namespace mer
{
	class Memory;
	extern Memory memory;
	namespace ins
	{
		template<typename _Ty>
		void add(const instruction_param& p)
		{
			data_cast<_Ty>(p.dest) = data_cast<_Ty>(p.first) + data_cast<_Ty>(p.second);
		}

		// first - push_value size, second- data
		void push(const instruction_param& p);
		void pop(const instruction_param& p);
		// p.first is exit code;
		void exit(const instruction_param& p);
	}
}