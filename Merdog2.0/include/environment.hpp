/*
*		Inspired by
*		https://ruslanspivak.com/lsbasi-part10/
*		Ruslan's Blog
*		C++ Version.
*		Yuantao Hu 2018
*		Email :Huyuantao@outlook.com
*/
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
		void push();
		void pop();
		void insert_basic(Token *tok, IdType v)
		{
			basic_map.front().insert({ tok,v });
		}
		void insert_type(Token *tok, size_t v)
		{
			type_map.front().insert({ tok,v });
		}
		IdType find_basic(Token *tok);
		size_t find_type(Token *tok);
	private:
		std::deque<std::map<Token*, IdType>> basic_map;
		std::deque<std::map<Token*, size_t>> type_map;
	};
	extern Symbol symbol_table;
}