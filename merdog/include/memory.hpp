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
			_mem = new Mem::Object[capacity];
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
		size_t push(int size)
		{
			index += size;
			check();
			return index;
		}
		size_t push()
		{
			check();
			return index++;
		}
		size_t end_block() {
			index = block_flag.back();
			block_flag.pop_back();
			return block_flag.back();
		}
		size_t get_current()
		{
			return current;
		}
		Mem::Object& operator[] (size_t in)
		{
			return _mem[in + current];
		}
		~Memory()
		{
			delete[] _mem;
		}
		size_t& get_index() {
			return index;
		}
	private:
		void check()
		{
			if (index+current> 0.5*capacity)
				alloc();
		}
		void alloc();
		size_t index = 0;
		size_t current = 0;
		size_t capacity = 1024;
		std::stack<size_t> call_stack;
		std::vector<size_t> block_flag;
		Mem::Object *_mem;
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
	extern Memory stack_memory;
	extern Heap heap_memory;
}