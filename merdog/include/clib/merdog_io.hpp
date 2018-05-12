#pragma once
#include "../function.hpp"
#include "../include/namespace.hpp"
namespace Mer
{
	class Namespace;
	extern Namespace *mstd;
	void set_io();
	extern Mer::SystemFunction *cout;
	extern Mer::SystemFunction *cls;
	extern Mer::SystemFunction *input_int;
	extern Mer::SystemFunction *input_real;
	extern Mer::SystemFunction *input_string;
}