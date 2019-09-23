#pragma once
#include <vector>
#include <stack>
#include <map>
#include <set>
#include "object.hpp"
namespace Mer
{
	//stack_mem include static_mem
	/*
		we use get_current() as off_set to get the stack_obj's add.
	*/
	class Memory
	{
	public:
		Memory()
		{
			stack_mem = new Mem::Object[capacity];
			block_flag.push_back(0);
		}
		size_t new_block()
		{
			block_flag.push_back(index);
			return index;
		}
		void new_func(int siz)
		{
			current += siz;
			call_stack.push(siz);
		}
		void end_func()
		{
			current -= call_stack.top();

			call_stack.pop();
		}
		size_t push(int size);
		size_t push();
		size_t end_block();
		size_t get_current()
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
	private:
		void check()
		{
			while (index+current> 0.5L*capacity)
				alloc();
		}
		//alloc for memory
		void alloc();
		size_t index = 0;
		size_t current = 0;
		size_t capacity = 2048;
		std::stack<size_t> call_stack;
		std::vector<size_t> block_flag;
		Mem::Object *stack_mem;
	};
	extern Memory mem;
}