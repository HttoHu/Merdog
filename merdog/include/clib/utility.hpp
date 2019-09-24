#pragma once
#include "merdog_io.hpp"
namespace Mer
{
	class Namespace;
	extern SystemFunction* time_record;
	extern SystemFunction* random_int;
	extern SystemFunction* sleep;
	extern SystemFunction* csystem;
	extern SystemFunction* alloc_heap;
	void set_utility();
}