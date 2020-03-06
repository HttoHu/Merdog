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

namespace Mer
{
	std::vector<std::vector<Mem::Value*>> Mem::del_obj_table(1);
	void Mem::swap(Object rhs, Object lhs)
	{
		auto tmp = rhs->clone();
		rhs->operator=(lhs);
		lhs->operator=(tmp);
	}
	std::string Mem::type_to_string(BasicType bt)
	{
		std::string ret;
		bool is_p = bt % 2 || bt == 0;
		is_p = !is_p;
		if (is_p)
			bt = BasicType(bt - 1);
		switch (bt)
		{
		case Mem::BVOID:
			ret += "void";
			break;
		case Mem::BOOL:
			ret += "bool";
			break;
		case Mem::INT:
			ret += "int";
			break;
		case Mem::DOUBLE:
			ret += "double";
			break;
		case Mem::STRING:
			ret += "string";
			break;
		case Mem::CHAR:
			ret += "char";
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


	Mem::Object Mem::create_var_t(type_code_index type)
	{
		// pointer
		if (type % 2 == 0)
		{
			return make_object<Pointer>(nullptr);
		}
		switch (type)
		{
		case CHAR:
			return make_object<Char>(0);
		case INT:
			return make_object<Int>(0);
		case DOUBLE:
			return make_object<Double>(0.0);
		case BOOL:
			return make_object<Bool>(true);
		case STRING:
			return make_object<String>("");
		default:
		{
			auto result = type_init_map.find(type);
			if (result == type_init_map.end())
			{

				return make_object<USObject>(find_ustructure_t(type)->init());
			}
			return result->second->clone();
		}
		}
	}

	Mem::Object Mem::Int::operator=(Object v)
	{
		auto tmp = v;
		value = static_cast<Int*>(v)->value;
		return tmp;
	}

	Mem::Object  Mem::Int::Convert(type_code_index type)
	{
		switch (type)
		{
		case INT:
			return make_object<Int>(value);
		case DOUBLE:
			return make_object<Double>(value);
		case BOOL:
			return make_object<Bool>(value);
		case CHAR:
			return make_object<Char>(value);
		default:
			throw Error("int_value:" + std::to_string(value) + " int cannot convert to " + type_to_string((BasicType)type));
			break;
		}
	}

	Mem::Object Mem::Double::operator=(Object v) {

		value = static_cast<Double*>(v)->value;
		return Convert(Mem::DOUBLE);
	}

	Mem::Object Mem::Double::Convert(type_code_index type)
	{
		switch (type)
		{
		case INT:
			return make_object<Int>(value);
		case DOUBLE:
			return make_object<Double>(value);
		default:
			throw Error("double cannot convert to " + type_to_string((BasicType)type));
		}
	}

	Mem::Object Mem::Bool::Convert(type_code_index type)
	{
		switch (type)
		{
		case BOOL:
			return make_object<Bool>(value);
		case INT:
			return make_object<Int>(value);
		default:
			throw Error("type-convert error");
		}
	}

	bool Mem::Type::convertible(const type_code_index& t)
	{
		return convertible_types.find(t) != convertible_types.end();
	}

	void Mem::Type::add_compatible_type(type_code_index type_code)
	{
		convertible_types.insert(type_code);
	}

	Mem::Pointer::Pointer(Object _obj) :obj(_obj)
	{
	}

	Mem::Object Mem::Pointer::operator=(Object v)
	{
		obj = static_cast<Pointer*>(v)->obj;
		return make_object<Pointer>(obj);
	}

	Mem::Object Mem::Pointer::operator==(Object v)
	{
		return make_object<Mem::Bool>(static_cast<Pointer*>(v)->obj == obj);
	}

	Mem::Object Mem::Pointer::operator!=(Object v)
	{
		return make_object<Mem::Bool>(static_cast<Pointer*>(v)->obj != obj);
	}

	Mem::Object Mem::Pointer::untagged_clone() const
	{
		return new Mem::Pointer(obj);
	}

	Mem::Object Mem::Pointer::clone() const
	{
		return make_object<Mem::Pointer>(obj);
	}

	Mem::Object Mem::Pointer::operator[](Object v)
	{
		return obj->operator[](v);
	}

	Mem::Pointer::~Pointer()
	{
	}

	Mem::Object Mem::String::operator[](Object v)
	{
		return make_object<Char>(&str[static_cast<Int*>(v)->get_value()]);
	}

	std::string type_to_string(size_t type_code)
	{
		return Mem::type_to_string(Mem::BasicType(type_code));
	}

	Mem::Object Mer::Mem::Char::operator=(Object v)
	{
		auto tmp = v;
		*value = *static_cast<Char*>(v)->value;
		return tmp;
	}

	Mem::Object Mer::Mem::Char::Convert(type_code_index type)
	{
		switch (type)
		{
		case STRING:
			return make_object<String>(*value);
		case BOOL:
			return make_object<Bool>(*value);
		case INT:
			return make_object<Int>(*value);
		case CHAR:
			return make_object<Char>(*value);
		default:
			throw Error("type-convert error");
		}
	}

	Mem::Object Mem::AnyObj::untagged_clone() const
	{
		return new AnyObj(obj);
	}

	Mem::Object Mer::Mem::AnyObj::clone() const
	{
		return make_object<AnyObj>(obj);
	}

	Mem::Object Mer::Mem::Array::operator[](Object index)
	{
		int i = Mem::get_raw<int>(index);
		if (i >= length)
		{
			throw std::runtime_error("array overflow!");
		}
		return mem[pos + i + 1+mem.get_current()];
	}

	Mem::Object Mem::Array::clone() const
	{
		return make_object<Array>(type, pos, length);
	}

	Mem::Object Mem::Array::untagged_clone() const
	{
		return new Array(type, pos, length);
	}

	Mem::Object Mem::Value::Convert(type_code_index type)
	{
		std::string ls = type_to_string((BasicType)get_type());
		std::string rs = type_to_string(BasicType(type));
		throw Error("convert: syntax error from " + ls + " to " + rs);
	}

	Mem::Object Mem::InitListObj::untagged_clone() const
	{
		std::vector<Object> vec;
		for (auto& a : elems)
		{
			vec.push_back(a->untagged_clone());
		}
		return new InitListObj(std::move(vec), type_code);
	}

	Mem::Object Mem::InitListObj::clone() const
	{
		std::vector<Object> vec;
		for (auto& a : elems)
		{
			vec.push_back(a->clone());
		}
		return make_object<InitListObj>(std::move(vec), type_code);
	}

	Mem::Object Mer::Mem::GArray::operator[](Object index)
	{
		int i = Mem::get_raw<int>(index);
		if (i >= length)
		{
			throw std::overflow_error("array overflow!");
		}
		return mem[pos + i + 1u];
	}

	Mem::Object Mem::GArray::clone() const
	{
		return make_object<GArray>(type, pos, length);
	}

}
