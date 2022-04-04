#include "../includes/branch_loop.hpp"
namespace Mer {
	void Goto::execute(char*)
	{
		*pc = dest;
	}
}