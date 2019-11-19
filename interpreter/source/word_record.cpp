/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/word_record.hpp"
#include "../include/function.hpp"
using namespace Mer;
std::map<size_t, std::map<std::string, size_t>> type_op_type_map;
WordRecorder* Mer::SymbolTable::find(std::string id)

{
	for (size_t i = 0; i < data.size(); i++)
	{
		auto result = data[i].find(id);
		if (result != data[i].end())
		{
			return result->second;
		}
	}
	return nullptr;
}
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

Mer::FuncIdRecorder::FuncIdRecorder(FunctionBase* fb) :WordRecorder(ESymbol::SFUN, fb->get_type()), function(fb) {}
