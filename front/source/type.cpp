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
		case mer::REAL_DECL:
			token_stream.next();
			return type_map["real"];
		case mer::BOOL_DECL:
			token_stream.next();
			return type_map["bool"];
		case mer::CHAR_DECL:
			token_stream.next();
			return type_map["char"];
		default:
			throw Error("unkown type");
		}

	}

	void create_value_type(std::string name, size_t length)
	{
		type_map.insert({ name,new Type(Value,name,length) });
	}
	void Type::set_convertible_type(const std::set<Type*>& _table)
	{
		convertible_set = _table;
		convertible_set.insert(this);
	}
	bool Type::same(Type* t)
	{
		return type_code == t->type_code;
	}
	bool Type::convertible(Type* t)
	{
		return convertible_set.find(t) != convertible_set.end();
	}
}