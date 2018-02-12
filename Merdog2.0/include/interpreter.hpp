#pragma once
#include "lexer.hpp"
#include "expr.hpp"
#include <iostream>
namespace Mer
{
    class Interpreter
    {
        public:
            Interpreter()
            {
            }
            void run()
            {
				Expr expr;
				std::cout << expr.get_value();
            }
        private:
    };
}