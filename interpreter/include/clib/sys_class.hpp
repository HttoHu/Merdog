#pragma once
#include <vector>
#include <map>
#include <set>
#include "../parser.hpp"

namespace Mer
{
	class FunctionBase;
	class StdClass
	{
	public:
		StdClass() {
			type_code = ++Mem::type_counter;
		}
		virtual FunctionBase* get_function(const std::string &id) { return nullptr; };
		virtual ~StdClass() {}
	private:
		std::map<std::string, FunctionBase*> member_info;
		size_t type_code;
	};
	extern std::map<std::string, StdClass*> std_class_map;

}
/*
*/