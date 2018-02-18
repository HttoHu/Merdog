#pragma once
#include <list>
#include <map>
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
		}
		size_t new_block() {
			block_flag.push_back(current);
		}
		size_t push()
		{
			check();
			return current++;
		}
		size_t end_block() {
			current = block_flag.back();
			block_flag.pop_back();
		}
		Mem::Object& operator[] (size_t index)
		{
			return _mem[index];
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
			for (int i = 0; i < current; i++)
			{
				tmp[i] = _mem[i];
			}
			delete[] _mem;
			_mem = tmp;
		}
		size_t current=0;
		size_t capacity = 128;
		std::vector<size_t> block_flag;
		Mem::Object *_mem;
	};
	extern Memory _mem;
	extern std::list<std::map<Mer::Token*, size_t>> id_pos_table;
	size_t find_pos(Token *tok);
}