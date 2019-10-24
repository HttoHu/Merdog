/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/type.hpp"
namespace Mer
{
	namespace Mem
	{
		int type_counter = 7;
		std::map<size_t, Mem::Type*> type_map
		{
			{ BasicType::INT,new Type("int",BasicType::INT,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE })},
			{ BasicType::DOUBLE,new Type("double",BasicType::DOUBLE,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE }) },
			{ BasicType::BOOL,new Type("bool",BasicType::BOOL,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE }) },
			{ BasicType::STRING,new Type("string",BasicType::STRING,{ 11,BasicType::STRING }) }
		};
		ComplexType::ComplexType(size_t ct, size_t et) :
			Type("no_name", (size_t)(type_counter), { (size_t)(++type_counter) })
			, container_type(ct), element_type(et) {};

	}
	std::map<std::string, size_t> Mem::type_index;
}