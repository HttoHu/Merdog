#pragma once
#include "Action.hpp"
namespace Simcc
{
	namespace Runtime
	{
		void Init(const std::string & str);
		void CreateFunctionTable();
		void Execute();
	}
}