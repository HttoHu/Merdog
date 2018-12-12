#include "../include/type.hpp"
#include "../include/lexer.hpp"
#include "../include/parser.hpp"
#include "../include/namespace.hpp"
#include "../include/word_record.hpp"
using namespace Mer;
using namespace Mem;
int Mer::Mem::type_counter = 7;
size_t &Mer::Mem::type_no()
{
	static size_t this_type_index = 10;
	return this_type_index;
}

Mer::Mem::Type * Mer::Mem::get_type_by_string(const std::string & str)
{
	auto result = tsymbol_table->find(str);
	Type *original_type = nullptr;
	if (result == nullptr)
		throw Error("type "+str+"no found");
	if (result->es == ESymbol::STYPE)
	{
		auto result2 = type_map.find(result->get_type());
		if (result2 == type_map.end())
		{
			throw Error("type " + str + "no found");
		}
		return result2->second;
	}
}

std::map<size_t, Mer::Mem::Type* > Mer::Mem::type_map
{
	{ BasicType::BVOID,new Mer::Mem::Type("void",BasicType::BVOID,{ BasicType::BVOID}) },
	{ BasicType::INT,new Mer::Mem::Type("int",BasicType::INT,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE })},
	{ BasicType::DOUBLE,new Type("double",BasicType::DOUBLE,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE }) },
	{ BasicType::BOOL,new Type("bool",BasicType::BOOL,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE }) },
	{ BasicType::STRING,new Type("int",BasicType::STRING,{}) }
};

bool Mer::Mem::Type::compatible(const Type * t)
{
	return convertible(t);
}

bool Mer::Mem::Type::convertible(const Type * ty)
{
	size_t t = ty->type_code;
	return convertible_types.find(t) != convertible_types.end();
}

void Mer::Mem::set_new_type(const std::string & type_name, Type * t)
{

}

std::string Mer::Mem::type_to_string(BasicType bt)
{
	switch (bt)
	{
	case Mer::Mem::BVOID:
		return "void";
	case Mer::Mem::BOOL:
		return "bool";
	case Mer::Mem::INT:
		return "int";
	case Mer::Mem::DOUBLE:
		return "double";
	case Mer::Mem::STRING:
		return "string";
	default:
		break;
	}
	return "";
}

bool Mer::Mem::CompoundType::compatible(const Type * t)
{	
	if (children.size() == 1)
	{
		return children[0]->convertible(t);
	}
	return false;
}

bool Mer::Mem::CompoundType::convertible(const Type * t)
{
	
	if (Type::convertible(t) == true)
	{
			
	}
}

Type * _get_next_type()
{
	auto name = Mer::String::get_value(token_stream.this_token());
	auto original_type = get_type_by_string(name);
	token_stream.match(ID);
	return original_type;
}

Mer::Mem::Type* Mer::Parser::parse_type()
{
	auto name = Mer::String::get_value(token_stream.this_token());
	auto original_type = get_type_by_string(name);
	token_stream.match(ID);
	if (token_stream.this_tag() == LT)
		token_stream.match(LT);
	else
		return original_type;
	std::vector<Type*> type_vec;
	while  (token_stream.this_tag() != GT)
	{
		auto arg = _get_next_type();
		if (token_stream.this_tag() == COMMA)
			token_stream.match(COMMA);
	}
	token_stream.match(COMMA);
	return new Mer::Mem::CompoundType(original_type, type_vec);

}
