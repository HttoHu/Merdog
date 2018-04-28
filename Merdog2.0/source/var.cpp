#include "../include/var.hpp"
#include "../include/memory.hpp"
#include "../include/lexer.hpp"
using namespace Mer::Mem;
std::string Mer::Mem::type_to_string(BasicType bt)
{
	switch (bt)
	{
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
}

size_t Mer::Mem::get_type_code(Token * tok)
{
	switch(tok->get_tag())
	{
	case INTEGER_DECL:
		return INT;
	case REAL_DECL:
		return DOUBLE;
	case BOOL_DECL:
		return BOOL;
	case STRING_DECL:
		return STRING;
	case ID:
		throw Error("not finished yet");
	}
}

Mer::Mem::Object  Mer::Mem::Int::Convert(int type)
{
	switch (type)
	{
	case INT:
		return std::make_shared<Int>(value);
	case DOUBLE:
		return std::make_shared<Double>(value);
	case BOOL:
		return std::make_shared<Bool>(value);
	default:
		throw Error("int cannot convert to " + type_to_string((BasicType)type));
		break;
	}
}

Object Mer::Mem::Double::Convert(int type)
{
	switch (type)
	{
	case INT:
		return std::make_shared<Int>(value);
	case DOUBLE:
		return std::make_shared<Double>(value);
	default:
		throw Error("double cannot convert to " + type_to_string((BasicType)type));
	}
}

Object Mer::Mem::Bool::Convert(int type)
{
	switch (type)
	{
	case BOOL:
		return std::make_shared<Bool>(value);
	case INT:
		return std::make_shared<Int>(value);
	default:
		throw Error("type-convert error");
	}
}