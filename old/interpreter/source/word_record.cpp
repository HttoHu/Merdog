/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/word_record.hpp"
#include "../include/function.hpp"
using namespace Mer;
std::map<type_code_index, std::map<std::string, type_code_index>> type_op_type_map;
void Mer::SymbolTable::end_block()
{
	for (auto& a : data.front())
		_rem_vec.push_back(a.second);
	data.pop_front();
}
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
void Mer::SymbolTable::push_glo(std::string id, WordRecorder* wr)
{
	if (data.back().find(id) != data.back().end())
		throw Error("id " + id + " redefined!");
	data.back().insert({ id,wr });
}
void Mer::SymbolTable::push(std::string id, WordRecorder* wr)
{
	if (data.front().find(id) != data.front().end())
		throw Error("id " + id + " redefined!");
	data.front().insert({ id,wr });
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

Mer::SymbolTable::~SymbolTable()
{
	for (auto a : _rem_vec)
	{
		delete a;
	}
	for (auto& a : data)
	{
		for (auto& b : a)
		{
			delete b.second;
		}
	}
}

FunctionBase* Mer::FuncIdRecorder::find(const std::vector<type_code_index>& pf)
{
	if (dnt_check)
		return functions[std::vector<type_code_index>()];
	if (functions.find(pf) == functions.end())
		return nullptr;
	return functions[pf];
}

Mer::FuncIdRecorder::FuncIdRecorder(FunctionBase* fb) :WordRecorder(ESymbol::SFUN, fb->get_type()), functions(compare_param_feature) {}

Mer::FuncIdRecorder::~FuncIdRecorder()
{
	for (auto& a : functions)
	{
		rem_functions.insert(a.second);
	}
}
