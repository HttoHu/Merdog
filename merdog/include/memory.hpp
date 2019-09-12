#pragma once
#include <vector>
#include <stack>
#include <map>
#include <set>
#include "object.hpp"
namespace Mer
{
	class Memory
	{
	public:
		Memory()
		{
			static_mem = new Mem::Object[scapacity];
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
		size_t push_to_static();
		size_t push_to_static(size_t sz);
		size_t end_block();
		size_t get_current()
		{
			return current;
		}
		Mem::Object& static_index(size_t in);
		Mem::Object& operator[]  (size_t in);
		~Memory()
		{
			delete[] stack_mem;
			delete[] static_mem;
		}
		size_t& get_index() {
			return index;
		}
	private:
		void check()
		{
			if (index+current> 0.5L*capacity)
				alloc();
		}
		void check_static();
		void alloc();
		//alloc for static memory
		void salloc();
		size_t index = 0;
		size_t current = 0;
		size_t capacity = 1024;
		size_t scapacity = 512;
		std::stack<size_t> call_stack;
		std::vector<size_t> block_flag;
		Mem::Object *stack_mem;
		Mem::Object* static_mem;

		size_t sindex=0;
		
	};
	class Heap
	{
	public:
		Heap()
		{
			hmem.reserve(100);
		}
		size_t index()
		{
			return hmem.size();
		}
		size_t capacity()
		{
			return hmem.capacity();
		}

		size_t push(Mem::Object obj)
		{
			if (deleted_space.size() != 0)
			{
				hmem[deleted_space.top()] = obj;
				deleted_space.pop();
			}
			else
				hmem.push_back(obj);
		}
		size_t del(size_t sz)
		{
			deleted_space.push(sz);
		}
	private:
		std::vector<Mem::Object> hmem;
		std::stack<size_t> deleted_space;
	};
	extern Memory mem;
	extern Heap heap_memory;
}