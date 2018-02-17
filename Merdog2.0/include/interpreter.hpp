#pragma once
#include "parser.hpp"
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
				Parser::parse()->get_value();
            }
        private:
    };
}