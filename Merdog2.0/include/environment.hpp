#pragma once
#include "parser.hpp"
namespace Mer
{
	enum IdType
	{
		TVar,TFunction,TNamespace
	};
	class Symbol
	{
	public:
		Symbol() {
			push();
		}
		void push() {
			basic_map.push_front(std::map<Token*, IdType>());
			type_map.push_front(std::map<Token*, size_t>());
		}
		void pop()
		{
			basic_map.pop_front();
			type_map.pop_front();
		}
		void insert_basic(Token *tok, IdType v)
		{
			basic_map.front().insert({ tok,v });
		}
		void insert_type(Token *tok, size_t v)
		{
			type_map.front().insert({ tok,v });
		}
		IdType find_basic(Token *tok)
		{
			for (const auto a : basic_map)
			{
				auto result = a.find(tok);
				if (result != a.end())
					return result->second;
			}
			throw Error("id " + tok->to_string() + "no found");
		}
		size_t find_type(Token *tok)
		{
			for (const auto a : type_map)
			{
				auto result = a.find(tok);
				if (result != a.end())
					return result->second;
			}
			throw Error("id " + tok->to_string() + "no found");
		}
	private:
		std::deque<std::map<Token*, IdType>> basic_map;
		std::deque<std::map<Token*, size_t>> type_map;
	};
	extern Symbol symbol_table;
}