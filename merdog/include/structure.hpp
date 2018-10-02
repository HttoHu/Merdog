#pragma once
#include <vector>
#include <map>
#include <set>
#include "parser.hpp"
namespace Mer
{
	enum MemberType
	{
		FUNC = 0, VAR = 1
	};
	class FunctionBase;
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
	bool compareTypeInfo(const TypeInfo &a, const TypeInfo &b);
	// to init build_in type such as vector..
	class StructureBase
	{
	public: 
		virtual FunctionBase* get_function(const std::string &id) {};
		virtual ~StructureBase() {}
	private:
	};
}