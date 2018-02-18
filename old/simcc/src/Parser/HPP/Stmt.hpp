#pragma once
#include "../../Runtime/HPP/Memory.hpp"
namespace Simcc
{
	namespace Parser
	{
		class Stmt
		{
		public:
			virtual void execute() {};
			virtual ~Stmt() {};
		};
	}
}