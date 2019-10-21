#include "../include/type.hpp"
namespace mer
{
	std::map<std::string, Type*> type_map;
	int type_counter=0;



	void create_value_type(std::string name, size_t length);
	{
		type_map.insert({ name,new Type(Value,name,length) });
	}
}