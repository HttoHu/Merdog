/*
*		Inspired by
*		https://ruslanspivak.com/lsbasi-part10/
*		Ruslan's Blog
*		C++ Version.
*		Yuantao Hu 2018
*		Email :Huyuantao@outlook.com
*/
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