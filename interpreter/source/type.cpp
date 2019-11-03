/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/type.hpp"
#include "../include/error.hpp"
namespace Mer
{
	namespace Mem
	{
		int type_counter = 9;
		std::map<size_t, std::map<std::string, size_t>> type_op_type_map
		{
			{Mem::STRING,{{"[]",Mem::CHAR}}}
		};
		std::map<size_t, Mem::Type*> type_map
		{
			{ BasicType::INT,new Type("int",BasicType::INT,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE })},
			{ BasicType::DOUBLE,new Type("double",BasicType::DOUBLE,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE }) },
			{ BasicType::BOOL,new Type("bool",BasicType::BOOL,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE }) },
			{ BasicType::STRING,new Type("string",BasicType::STRING,{ 11,BasicType::STRING }) },
			{BasicType::CHAR,new Type("char",BasicType::CHAR,{9,BasicType::CHAR,BasicType::INT,BasicType::STRING})}
		};
		ComplexType::ComplexType(size_t ct, size_t et) :
			Type("no_name", (size_t)(type_counter), { (size_t)(++type_counter) })
			, container_type(ct), element_type(et) {};

		size_t find_op_type(size_t ty, std::string op)
		{
			auto result1 = type_op_type_map.find(ty);
			if (result1 == type_op_type_map.end())
				throw Error("type "+std::to_string(ty) + " hasn't registerd the operator "+op);
			auto result2= result1->second.find(op);
			if(result2==result1->second.end())
				throw Error("type " + std::to_string(ty) + " doesn't support the operator " + op);
			return result2->second;
		}

	}
	std::map<std::string, size_t> Mem::type_index;
}