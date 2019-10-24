#include "../include/symbol_table.hpp"
#include "../include/type.hpp"
namespace mer
{ 
	// symbol table for user
SymbolTable _symbol_table;
std::string get_tmp_var_name(bool c)
{
	static int index = 0;
	if (c)
	{
		index = 0;
		return "";
	}
	return "%" + std::to_string(index++);
}

WordRecorder* find_recorder_by_id(const std::string& str)
{
	auto result = _symbol_table.find(str);
	if (result == nullptr)
		throw Error(" id: " + str + " hasn't been defined yet!");
	return result;
}

void SymbolTable::type_check(Token* id, WordRecorder::SymbolTag e)
{
	auto result = find(Id::get_value(id));
	if (result == nullptr)
		throw Error("id " + id->to_string() + " no found.");
	if (result->symbol_type != e)
		throw Error("id typ not matched");
}
WordRecorder* SymbolTable::find_front(std::string id)
{
	auto result = data.front().find(id);
	if (result == data.front().end())
		return nullptr;
	return result->second;
}

WordRecorder* SymbolTable::find(std::string id)
{
	for (int i = 0; i < data.size(); i++)
	{
		auto result = data[i].find(id);
		if (result != data[i].end())
		{
			return result->second;
		}
	}
	return nullptr;
}
void SymbolTable::print()
{
	for (const auto& a : data)
	{
		for (const auto& b : a)
		{
			std::cout << "ID:" << b.first << " TYPE:" << b.second->type->name() << std::endl;;
		}
		std::cout << "=================================\n";
	}
	std::cout << "#########################################\n\n\n";
}

}