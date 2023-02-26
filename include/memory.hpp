#pragma once
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
/*
	function_model: 
	mem-> |begin: local variable, program:| local vars...|....
	|function_call2:[current] args, local vars|function_call1 :args ,local vars|
	function call all pushed back, glo var are all in the front.

*/
#include <vector>
#include <stack>
#include <map>
#include <set>
#include "basic_objects.hpp"

namespace Mer
{
	const int mem_max_size = 4096;
	class Memory
	{
	public:
		Memory()
		{
			stack_mem = new Mem::Object[capacity];
			block_flag.push_back(0);
		}
		size_t new_block();
		void new_func(size_t off);
		void end_func();
		size_t push(int size);
		size_t push();
		size_t end_block();
		size_t& get_current()
		{
			return current;
		}
		Mem::Object& operator[]  (size_t in);
		~Memory()
		{
			delete[] stack_mem;
		}
		size_t& get_index() {
			return index;
		}
		void reset_func_block_size() {
			function_block_size = 0;
		}
		int function_block_size=0;
		size_t get_capacity() { return capacity; }

		size_t reserve_glo_pos(size_t size);
		void reset();
		void check()
		{
			while (index + current > capacity / 2)
			{
				alloc();
				capacity *= 2;
			}
		}
	private:
		size_t glo_var_index = 0;
		//alloc for memory
		void alloc();
		// how many var's the funciton contains;

		size_t index = 0;
		size_t current = 0;
		size_t capacity = mem_max_size;
		std::stack<size_t> call_stack;
		std::vector<size_t> block_flag;
		Mem::Object *stack_mem;
	};
	extern Memory mem;
}