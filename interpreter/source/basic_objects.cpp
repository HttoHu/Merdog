/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/basic_objects.hpp"
#include "../include/memory.hpp"
#include "../include/lexer.hpp"
#include "../include/namespace.hpp"
#include "../include/function.hpp"
#include "../include/word_record.hpp"
#include "../include/compound_box.hpp"
using namespace Mer::Mem;


std::string Mer::Mem::type_to_string(BasicType bt)
{
	std::string ret;
	bool is_p = bt % 2 || bt == 0;
	is_p = !is_p;
	if (is_p)
		bt = BasicType(bt - 1);
	switch (bt)
	{
	case Mer::Mem::BVOID:
		ret += "void";
		break;
	case Mer::Mem::BOOL:
		ret += "bool";
		break;
	case Mer::Mem::INT:
		ret += "int";
		break;
	case Mer::Mem::DOUBLE:
		ret += "double";
		break;
	case Mer::Mem::STRING:
		ret += "string";
		break;
	default:
		auto seeker = type_name_mapping.find(bt);
		if (seeker == type_name_mapping.end())
		{
			ret += ("!unkown type :" + std::to_string(bt));
		}
		else
			ret = seeker->second;
		break;
	}
	if (is_p)
		ret += "* ";
	return ret;
}


Mer::Mem::Object Mer::Mem::create_var_t(size_t type)
{

	// pointer
	if (type % 2 == 0)
	{
		return std::make_shared<Pointer>(nullptr);
	}
	switch (type)
	{
	case CHAR:
		return std::make_shared<Char>(0);
	case INT:
		return std::make_shared<Int>(0);
	case DOUBLE:
		return std::make_shared<Double>(0.0);
	case BOOL:
		return std::make_shared<Bool>(true);
	case STRING:
		return std::make_shared<String>("");
	default:
	{
		auto result = type_init_map.find(type);
		if (result == type_init_map.end())
		{
			return std::make_shared<USObject>(find_ustructure_t(type)->init());
		}
		return result->second->clone();
	}
	}
}

Object Mer::Mem::Int::operator=(Object v)
{
	auto tmp = v;
	value = std::static_pointer_cast<Int>(v)->value;
	return tmp;
}

Mer::Mem::Object  Mer::Mem::Int::Convert(size_t type)
{
	switch (type)
	{
	case INT:
		return std::make_shared<Int>(value);
	case DOUBLE:
		return std::make_shared<Double>(value);
	case BOOL:
		return std::make_shared<Bool>(value);
	case CHAR:
		return std::make_shared<Char>(value);
	default:
		throw Error("int_value:" + std::to_string(value) + " int cannot convert to " + type_to_string((BasicType)type));
		break;
	}
}

Object Mer::Mem::Double::operator=(Object v) {

	value = std::static_pointer_cast<Double>(v)->value;
	return Convert(Mem::DOUBLE);
}

Mer::Mem::Object Mer::Mem::Double::Convert(size_t type)
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

Mer::Mem::Object Mer::Mem::Bool::Convert(size_t type)
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

bool Mer::Mem::Type::convertible(const size_t& t)
{
	return convertible_types.find(t) != convertible_types.end();
}

void Mer::Mem::Type::add_compatible_type(size_t type_code)
{
	convertible_types.insert(type_code);
}

Mer::Mem::Pointer::Pointer(Object _obj) :obj(_obj)
{
}

Mer::Mem::Object Mer::Mem::Pointer::operator=(Object v)
{
	obj = std::static_pointer_cast<Pointer>(v)->obj;
	return std::make_shared<Pointer>(obj);
}

Mer::Mem::Object Mer::Mem::Pointer::operator==(Object v)
{
	return std::make_shared<Mem::Bool>(std::static_pointer_cast<Pointer>(v)->obj == obj);
}

Mer::Mem::Object Mer::Mem::Pointer::operator!=(Object v)
{
	return std::make_shared<Mem::Bool>(std::static_pointer_cast<Pointer>(v)->obj != obj);
}

Mer::Mem::Object Mer::Mem::Pointer::clone() const
{
	return std::make_shared<Mem::Pointer>(obj);
}

Object Mer::Mem::Pointer::operator[](Object v)
{
	return obj->operator[](v);
}

Mer::Mem::Pointer::~Pointer()
{
}

Object Mer::Mem::String::operator[](Object v)
{
	return std::make_shared<Char>(str[std::static_pointer_cast<Int>(v)->get_value()]);
}

std::string Mer::type_to_string(size_t type_code)
{
	return Mem::type_to_string(Mem::BasicType(type_code));
}

Object Mer::Mem::Char::operator=(Object v)
{
	auto tmp = v;
	value = std::static_pointer_cast<Char>(v)->value;
	return tmp;
}

Object Mer::Mem::Char::Convert(size_t type)
{
	switch (type)
	{
	case STRING:
		return std::make_shared<String>(value);
	case BOOL:
		return std::make_shared<Bool>(value);
	case INT:
		return std::make_shared<Int>(value);
	case CHAR:
		return std::make_shared<Char>(value);
	default:
		throw Error("type-convert error");
	}
}

Mer::Mem::Object Mer::Mem::AnyObj::clone() const
{
	return std::make_shared<AnyObj>(obj);
}

Mer::Mem::Object Mer::Mem::Array::operator[](Object index)
{
	int i = Mem::get_raw<int>(index);
	if (i >= length)
	{
		throw std::runtime_error("array overflow!");
	}
	return mem[size_t(pos) + size_t(i) + 1];
}

Mer::Mem::Object Mer::Mem::Array::clone() const
{
	return std::make_shared<Array>(type, pos, length);
}
