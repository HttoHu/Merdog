#include "../include/environment.hpp"
namespace mer
{
	// def some vars.
	size_t pc = 0;
	int64_t reg::rsp = 0;
	std::vector<std::pair<instruction, instruction_param >> ins_table;
	void run()
	{
		try
		{
			while (pc!= ins_table.size())
			{
				ins_table[pc].first(ins_table[pc].second);
				pc++;
			}
		}
		catch (size_t a)
		{
			std::cout << "program has exited, exit code " << a<<std::endl;
		}
	}
}