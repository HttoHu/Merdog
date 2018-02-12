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
				std::cout << std::endl;
				Parser::print_var_list();
            }
        private:
    };
}