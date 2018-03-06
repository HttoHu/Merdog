/*
* Inspired by
* https://ruslanspivak.com/lsbasi-part10/
* Ruslan's Blog
* C++ Version.
* Yuantao Hu 2018
*/
#include "../include/memory.hpp"
Mer::Memory Mer::_mem;
std::list<std::map<Mer::Token*, std::size_t>> Mer::id_pos_table{ std::map<Mer::Token*,size_t>() };

size_t Mer::find_pos(Token * tok)
{
	for (auto &a : id_pos_table)
	{
		auto result = a.find(tok);
		if (result != a.end())
		{
			return result->second;
		}
	}
	throw Error(tok->to_string() + " no found");
}
