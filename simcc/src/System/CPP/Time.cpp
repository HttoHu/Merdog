#include "..\HPP\Time.hpp"
#include <random>
Simcc::Runtime::ObjectBase * Simcc::System::clock(Parser::Param * param)
{
	if (param->param_list.size() != 0)
		throw Error::SyntaxError("param not matched");

	double r = (double)::clock()/CLK_TCK;
	return new Simcc::Runtime::ObjectBase(r);
}

Simcc::Runtime::ObjectBase * Simcc::System::random_int(Parser::Param * param)
{
	if (param->param_list.size() != 2)
		throw Error::SyntaxError("param not matched");
}
