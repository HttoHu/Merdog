/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/word_record.hpp"
using namespace Mer;

void Mer::SymbolTable::print()
{
	for (const auto& a : data)
	{
		for (const auto& b : a)
		{
			std::cout << "ID:" << b.first << " TYPE:" << b.second->get_type() << std::endl;;
		}
		std::cout << "=================================\n";
	}
	std::cout << "#########################################\n\n\n";
}
