/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/namespace.hpp"
#include "../include/value.hpp"
#include "../include/word_record.hpp"
using namespace Mer;
Namespace *Mer::root_namespace = new Namespace(nullptr);
Namespace *Mer::this_namespace = root_namespace;
//========================================================================
Mer::Namespace::Namespace(Namespace * pare) :parent(pare)
{
	sl_table = new SymbolTable();
}
/*
void Mer::Namespace::set_new_structure(const std::string & name, Structure * structure)
{
	structures.insert({ name,{structure,type_counter++} });
	sl_table->push_glo(name, new WordRecorder(ESymbol::SSTRUCTURE));
}*/
void Mer::Namespace::set_new_func(const std::string & name, FunctionBase * func)
{
	functions.insert({ name,func });
	sl_table->push_glo(name, new FuncIdRecorder(func->get_type()));
}
void Mer::Namespace::set_new_var(const std::string & name, size_t type, Mem::Object obj)
{
	//sl_table->push_glo(name, new GVarIdRecorder(type, obj));
}

Mer::Mem::Object Mer::Namespace::find_var(const std::string & name)
{
	auto result = sl_table->find(name);
	if (result != nullptr)
	{
		if (result->es == ESymbol::SGVAR)
			return static_cast<GVarIdRecorder*>(result)->get_value();
		else
			throw Error("Error A20");
	}
	if (parent != nullptr)
		return parent->find_var(name);
	else
		throw Error("var " + name + " no found");
}

Mer::FunctionBase * Mer::Namespace::find_func(const std::string & name)
{
	auto result = functions.find(name);
	if (result != functions.end())
		return result->second;
	if (parent != nullptr)
		return parent->find_func(name);
	else
		throw Error("function " + name + " no found");
}

Mer::Namespace * Mer::Parser::_find_namespace_driver(Mer::Namespace *current, const std::string &name)
{
	auto result = current->children.find(name);
	if (result == current->children.end())
	{
		if (current->parent == nullptr)
			return nullptr;
		return _find_namespace_driver(current->parent, name);
	}
	return result->second;
}

Mer::Namespace * Mer::Parser::find_namespace(const std::string & name)
{
	auto result = _find_namespace_driver(this_namespace, name);
	if (result == nullptr)
		return nullptr;
	return result;
}

