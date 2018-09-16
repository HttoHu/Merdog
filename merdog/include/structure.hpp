#pragma once
#include <vector>
#include <map>
#include "parser.hpp"
namespace Mer
{
	enum MemberType
	{
		FUNC = 0, VAR = 1
	};

	class TypeInfo
	{
	public:
		TypeInfo(const std::string &k, MemberType _info) :key(k), info(_info) {}
		std::string get_key()const { return key; }
		MemberType get_info()const  { return info; }
	private:
		std::string key;
		MemberType info;
	};
	bool compareTypeInfo(const TypeInfo &a, const TypeInfo &b)
	{
		a.get_key() < b.get_key();
	}
	// to init build_in type such as vector,,
	class BStructure
	{
	public:
		Mem::Object call(const std::string &func_name, std::vector<Mem::Object> &objs);
		Mem::Object var_table(size_t index);
	private:
		std::set <TypeInfo, decltype(compareTypeInfo)*> index_table;
		std::map <std::string, size_t> var_table;
		std::map <std::string, FunctionBase*> func_table;
		std::vector<Mer::Mem::Object> var_stack;
	};
}