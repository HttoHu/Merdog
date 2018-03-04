#include "../include/environment.hpp"
#include "../include/memory.hpp"
using namespace Mer;
Symbol Mer::symbol_table;

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
