/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/namespace.hpp"
#include "../include/value.hpp"
#include "../include/word_record.hpp"
using namespace Mer;
Namespace* Mer::root_namespace = new Namespace(nullptr);
Namespace* Mer::this_namespace = root_namespace;
//========================================================================
Mer::Namespace::Namespace(Namespace* pare) :parent(pare)
{
	sl_table = new SymbolTable();
}
/*
void Mer::Namespace::set_new_structure(const std::string & name, Structure * structure)
{
	structures.insert({ name,{structure,type_counter++} });
	sl_table->push_glo(name, new WordRecorder(ESymbol::SSTRUCTURE));
}*/
void Mer::Namespace::set_new_func(const std::string& name, FunctionBase* func)
{
	if (name == "compare")
	{
		// compare function which apply to map, set and some other dictionaries.
		if (func->param_types.size() == 2 && func->param_types[0] == func->param_types[1] && func->get_type() == Mem::BOOL)
		{
			auto lam = [=](Mem::Object obj1, Mem::Object obj2) {
				return std::static_pointer_cast<Mem::Bool>(func->run(std::vector<Mem::Object>{ obj1, obj2 }))->_value();
			};
			compare_map.insert({ func->param_types[0],lam });
		}
	}
	auto result = sl_table->find(name);
	if (result == nullptr)
	{
		result = new FuncIdRecorder(func->get_type());
		sl_table->push_glo(name,result);
	}
	auto recorder = static_cast<FuncIdRecorder*>(result);
	if (func->is_check_type() == false)
	{
		recorder->dnt_check = true;
		recorder->functions.insert({ std::vector<type_code_index>(),func });
		return;
	}
	recorder->functions.insert({ func->param_types,func });
}
void Mer::Namespace::set_new_var(const std::string& name, size_t type, Mem::Object obj)
{
	//sl_table->push_glo(name, new GVarIdRecorder(type, obj));
}

Mer::Mem::Object Mer::Namespace::find_var(const std::string& name)
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

Mer::Namespace::~Namespace()
{
	delete sl_table;
}


Mer::Namespace* Mer::Parser::_find_namespace_driver(Mer::Namespace* current, const std::string& name)
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

Mer::Namespace* Mer::Parser::find_namespace(const std::string& name)
{
	auto result = _find_namespace_driver(this_namespace, name);
	if (result == nullptr)
		return nullptr;
	return result;
}

