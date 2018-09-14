#pragma once
#include <vector>
#include <stack>
#include <map>
#include <set>
#include "var.hpp"
#define CALL_STACK_SIZE 100
#define BLOCK_MAX_DEEP 100
namespace Mer
{
	class Memory
	{
	public:
		Memory()
		{
			call_stack2 = new size_t[CALL_STACK_SIZE];
			_mem = new Mem::Object[capacity];
			block_flag2 = new size_t[BLOCK_MAX_DEEP];
		}
		size_t new_block();
		size_t end_block();
		void new_func(int siz);
		void end_func();
		size_t push();

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
			// well I hope they are moved to orginal position, yep, it should be.
			delete[] _mem;
			delete[] call_stack2;
			delete[] block_flag2;
		}
		size_t& get_index() {
			return index;
		}
	private:
		void check()
		{
			if (current > 0.5*capacity)
				alloc();
		}
		void alloc()
		{
			capacity *= 2;
			Mem::Object *tmp = new Mem::Object[capacity];
			for (size_t i = 0; i < current; i++)
			{
				tmp[i] = _mem[i];
			}
			delete[] _mem;
			_mem = tmp;
		}
		size_t index = 0;
		size_t current = 0;
		size_t capacity = 128;
		size_t *call_stack2=nullptr;
		//std::stack<size_t> call_stack;
		size_t *block_flag2 = nullptr;
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