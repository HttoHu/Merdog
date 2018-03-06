/*
	* Inspired by
	* https://ruslanspivak.com/lsbasi-part10/
	* Ruslan's Blog
	* C++ Version.
	* Yuantao Hu 2018
*/
#include "../include/environment.hpp"
#include "../include/memory.hpp"
using namespace Mer;
Symbol Mer::symbol_table;

void Mer::Symbol::push()
{
	//Mer::id_pos_table.push_front(std::map<Mer::Token*, size_t>());
	basic_map.push_front(std::map<Token*, IdType>());
	type_map.push_front(std::map<Token*, size_t>());
}
void Mer::Symbol::pop()
{
	basic_map.pop_front();
	type_map.pop_front();
	//id_pos_table.pop_front();
}
IdType Mer::Symbol::find_basic(Token * tok)
{
	for (const auto a : basic_map)
	{
		auto result = a.find(tok);
		if (result != a.end())
			return result->second;
	}
	throw Error("<id pos:" + std::to_string(find_pos(tok)) + ">" + tok->to_string() + "no found");
}

size_t Mer::Symbol::find_type(Token * tok)
{
	for (const auto a : type_map)
	{
		auto result = a.find(tok);
		if (result != a.end())
			return result->second;
	}
	throw Error("<id pos : " + std::to_string(find_pos(tok)) + ">" + tok->to_string() + "no found");
}
