#pragma once
#include <list>
#include <map>
#include <stack>
#include "lexer.hpp"
#include "type.hpp"
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
		size_t new_block() {
			block_flag.push_back(index);
			return index;
		}
		void new_func(int size_t)
		{
			current += size_t;
			call_stack.push(size_t);
		}
		void end_func()
		{
			current -= call_stack.top();
			call_stack.pop();
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
		Mem::Object& operator[] (size_t in)
		{
			return _mem[in+current];
		}
		~Memory()
		{
			delete[] _mem;
		}
		size_t& get_index(){
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
		size_t index=0;
		size_t current=0;
		size_t capacity = 128;
		std::stack<size_t> call_stack;
		std::vector<size_t> block_flag;
		Mem::Object *_mem;
	};
	extern Memory _mem;
	extern std::list<std::map<Mer::Token*, size_t>> id_pos_table;
	size_t find_pos(Token *tok);
}