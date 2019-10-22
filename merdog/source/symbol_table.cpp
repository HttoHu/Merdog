#include "../include/symbol_table.hpp"

std::string mer::get_tmp_var_name(bool c)
{
	static int index = 0;
	if (c)
	{
		index = 0;
		return "";
	}
	return "%" + std::to_string(index++);
}
