#include "../include/environment.hpp"
#include "../include/type.hpp"
namespace mer
{
	// def some vars.
	int64_t Reg[16] = { 0 };
	std::vector<std::pair<instruction, instruction_param >> ins_table;
	void init_merdog()
	{
		create_value_type("int",sizeof(int));
		create_value_type("real",sizeof(double));
		create_value_type("char",sizeof(char));
		create_value_type("bool",sizeof(bool));
	}
	void execute()
	{
		init_merdog();
		try
		{
			while (pc!= ins_table.size())
			{
				ins_table[pc].first(ins_table[pc].second);
				pc++;
			}
		}
		catch (int a)
		{
			std::cout << "program has exited, exit code " << a<<std::endl;
		}
	}
}