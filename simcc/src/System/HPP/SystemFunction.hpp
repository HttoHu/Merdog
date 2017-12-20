#pragma once
#include "../../Parser/HPP/Function.hpp"
#include "ConsoleIO.hpp"
#include <functional>
namespace Simcc
{
	namespace System
	{
		class CPPFunction:public Parser::Stmt
		{
		public:
			CPPFunction();
			~CPPFunction()
			{
				delete param;
			}
			void execute()override;
			static std::map <Lexer::Token*, std::function<Runtime::ObjectBase*(Parser::Param*)>> system_function_table;
		private:
			Lexer::Token *func_id;
			Parser::Param* param;
		};
	}
}