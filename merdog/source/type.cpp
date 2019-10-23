#include "../include/type.hpp"
#include "../include/lexer.hpp"
namespace mer
{
	std::map<std::string, Type*> type_map;
	int type_counter=0;



	Type* process_type()
	{
		switch (token_stream.this_tag())
		{
		case mer::INTEGER_DECL:
			token_stream.next();
			return type_map["int"];
		default:
			throw Error("unkown type");
		}

	}

	void create_value_type(std::string name, size_t length)
	{
		type_map.insert({ name,new Type(Value,name,length) });
	}
}