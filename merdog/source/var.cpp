#include "../include/var.hpp"
#include "../include/memory.hpp"
#include "../include/lexer.hpp"
#include "../include/namespace.hpp"
#include "../include/word_record.hpp"
using namespace Mer::Mem;
int Mer::type_counter=8;

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

size_t Mer::Mem::get_type_code(Token * tok)
{
	switch(tok->get_tag())
	{
	case VOID_DECL:
		return BVOID;
	case INTEGER_DECL:
		return INT;
	case REAL_DECL:
		return DOUBLE;
	case BOOL_DECL:
		return BOOL;
	case STRING_DECL:
		return STRING;
	case ID:
	{
		auto info = Mer::this_namespace->sl_table->find(Id::get_value(token_stream.this_token()));
		return info->get_type();
	}
	default:
		throw Error("invalid type ");
	}
}

size_t &Mer::Mem::type_no()
{
	static size_t this_type_index = 10;
	return this_type_index;
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

Mer::Mem::Object Mer::Mem::Double::Convert(int type)
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

Mer::Mem::Object Mer::Mem::Bool::Convert(int type)
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