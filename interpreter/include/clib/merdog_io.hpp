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
#include "../type.hpp"
#include "../function.hpp"
#include "../namespace.hpp"
//#define COUTTOSTRING
// offer the most basic io method and string operation
namespace Mer
{
	class Namespace;
	extern Namespace *mstd;
	void set_io();
	extern Mer::SystemFunction *substr;
	// to get the size  of a string
	extern Mer::SystemFunction *str_size;
	extern Mer::SystemFunction *cout;
	// clear the screen
	extern Mer::SystemFunction *cls;
	// get a int value from the input stream
	extern Mer::SystemFunction *input_int;
	// get a double value from the input stream
	extern Mer::SystemFunction *input_real;
	// get a std::string value from the input stream
	extern Mer::SystemFunction *input_string;
	extern Mer::SystemFunction *input_char;
}