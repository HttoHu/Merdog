#pragma once
#include <string>
#include <vector>
#include <map>
#include "../../Lexer/HPP/Token.hpp"
#include "Object.hpp"
namespace Simcc
{
	namespace Runtime
	{
		// 先写在这里吧, 改一下估计大半代码就要改了. 改了以后估计能带来大幅度性能的提升. 等我将语法稍稍完善后再改.
		// 还好只要表达式改改就好了.
		class Stack{
		public:
			Stack()
			{
				newBlock();
			}
			void newBlock();
			void endBlock();
			ObjectBase* find_variable(Lexer::Token* );
			void push(Lexer::Token* , ObjectBase *obj);
			void debug();
		private:
			std::list<std::map<Lexer::Token*,ObjectBase*>> stack_memory;
		};
	}
}