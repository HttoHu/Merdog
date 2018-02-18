#pragma once
#include <time.h>
#include "../../Parser/HPP/Function.hpp"
namespace Simcc
{
	namespace System
	{
		Simcc::Runtime::ObjectBase* clock(Parser::Param *param);
		Simcc::Runtime::ObjectBase* random_int(Parser::Param *param);
	}
}