#pragma once
#include "../type.hpp"
#include "../function.hpp"
#include "../namespace.hpp"
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
}