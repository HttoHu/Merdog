#pragma once
#include "Block.hpp"
#include "If.hpp"
#include "Single.hpp"
namespace Simcc
{
	namespace Parser
	{
		class Block;
		class While :public Stmt
		{
		public:
			While();
			void execute()override;
			~While();
		private:
			Expression *condition;
			Block *block;
		};
		class For:public Stmt
		{
		public:
			For();
			void execute()override;
			~For();
		private:
			CreateBasicTypeObject *cbt;
			Expression *condition;
			Runtime::Action *step_action;
			Block *block;
		};
	}
}