#pragma once
#include "merdog_io.hpp"
// some utility methods here.
namespace Mer
{
	class Namespace;
	// to record time;
	extern SystemFunction* time_record;
	extern SystemFunction* random_int;
	extern SystemFunction* sleep;
	// call command order
	extern SystemFunction* csystem;
	// you can alloc_heap by hand.
	extern SystemFunction* alloc_heap;
	// disabled method, don't use!.
	extern SystemFunction* type;
	void set_utility();
}