#include "expr.hpp"
namespace mer
{
	std::string IntV::get_gen()
	{
		return "int" + std::to_string(value);
	}
}
