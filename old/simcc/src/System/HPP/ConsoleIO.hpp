#pragma once
#include "../../Parser/HPP/Function.hpp"
namespace Simcc
{
	namespace System
	{
		Simcc::Runtime::ObjectBase* console_output(Parser::Param *param);
		Simcc::Runtime::ObjectBase* console_output_line(Parser::Param *param);
		Simcc::Runtime::ObjectBase* console_input(Parser::Param *param);
	}
}