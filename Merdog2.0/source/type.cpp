#include "../include/type.hpp"
using namespace Mer::Mem;
std::string Mer::Mem::type_to_string(BasicType bt)
{
	switch (bt)
	{
	case Mer::Mem::INT:
		return "int";
	case Mer::Mem::DOUBLE:
		return "double";
	case Mer::Mem::STRIHG:
		return "string";
	default:
		break;
	}
}

Mer::Mem::Value * Mer::Mem::Int::Convert(int type)
{
	switch (type)
	{
	case INT:
		return new Int(value);
	case DOUBLE:
		return new Double(value);
	default:
		throw Error("int cannot convert to " + type_to_string((BasicType)type));
		break;
	}
}

Value * Mer::Mem::Double::Convert(int type)
{
	switch (type)
	{
	case INT:
		return new Int(value);
	case DOUBLE:
		return new Double(value);
	default:
		throw Error("double cannot convert to " + type_to_string((BasicType)type));
	}
}
