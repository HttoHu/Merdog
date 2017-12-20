#pragma once
#include "Environment.hpp"
#include "Expression.hpp"
#include "CreateObject.hpp"
#include "Single.hpp"
namespace Simcc
{
	namespace Parser
	{
		class Block;
		class If :public Stmt
		{
		public:
			If();
			void execute()override;
			~If();
		private:
			std::list<std::pair<Expression*,Block *>>block;
			Block* Else=nullptr;
		};
	}
}