/*
		MIT License

		Copyright (c) 2019 HttoHu

		Permission is hereby granted, free of charge, to any person obtaining a copy
		of this software and associated documentation files (the "Software"), to deal
		in the Software without restriction, including without limitation the rights
		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the Software is
		furnished to do so, subject to the following conditions:

		The above copyright notice and this permission notice shall be included in all
		copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
		SOFTWARE.

*/
#pragma once
#include <typeinfo>
#include <iostream>
#include "parser_node.hpp"
#include "lexer.hpp"
namespace Mer
{
	namespace optimizer
	{
		extern std::map<Mer::Tag, Mem::Object(*) (const Mem::Object&, const Mem::Object&)> op_table;
		ParserNode* optimize_bin_op(ParserNode* left, ParserNode* right, Token* tok);
		ParserNode* optimize_unary_op(ParserNode* left, Token* tok);
		// try to obtain the address directly
		ParserNode* optimize_array_subscript(ParserNode* arr, ParserNode* subscript);
	}
}