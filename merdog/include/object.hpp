#pragma once
#include <string>
#include <memory>
#include <set>
#include <map>
#include <vector>
#include "type.hpp"
#include "error.hpp"
#define BASICTYPE_MAX_CODE 7
namespace Mer
{
	class StructureBase;
	class Namespace;
	class Token;
	namespace Mem
	{	

		class Type;
		class Value;
		using Object = std::shared_ptr<Value>;

		extern Namespace *this_namespace;

		std::string type_to_string(BasicType bt);
		size_t get_type_code(Token *tok);
		size_t &type_no();
		// to get a compound type's code like vector<map<int,real>>
		size_t get_ctype_code();
		size_t merge_two_type(size_t c, size_t e);
		Mem::Object create_var_t(size_t type);
		class Value
		{
		public:
			virtual Object clone()const { return nullptr; }
			virtual std::string to_string()const { return ""; }
			virtual size_t get_type()const
			{
				return BasicType::NDEF;
			}
			virtual Object operator=(Object v)
			{
				return nullptr;
			}
			virtual Object operator+=(Object v)
			{
				throw Error("syntax error");
			}
			virtual Object operator-=(Object v)
			{
				throw Error("syntax error");
			}
			virtual Object operator*=(Object v)
			{
				throw Error("syntax error");
			}
			virtual Object operator/=(Object v)
			{
				throw Error("syntax error");
			}
			virtual Object operator+ (Object v) { throw Error("syntax error"); }
			virtual Object operator- (Object v) { throw Error("syntax error"); }
			virtual Object operator* (Object v) { throw Error("syntax error"); }
			virtual Object operator/ (Object v) { throw Error("syntax error"); }
			virtual Object Convert(int type) { throw Error("syntax error"); }
			virtual Object get_negation()
			{
				throw Error("syntax error");
			}
			virtual Object operator>(Object v) { throw Error("syntax error"); }
			virtual Object operator<(Object v) { throw Error("syntax error"); }
			virtual Object operator>=(Object v) { throw Error("syntax error"); }
			virtual Object operator<=(Object v) { throw Error("syntax error"); }
			virtual Object operator !=(Object v) { throw Error("syntax error"); }
			virtual Object operator ==(Object v) { throw Error("syntax error"); }
			virtual Object operator[](Object v) { throw Error("syntax error"); }
			virtual Object operator&& (Object v) { throw Error("syntax error"); }
			virtual Object operator|| (Object v) { throw Error("syntax error"); }
			virtual ~Value() {}
		private:
		};
		class Int;
		class Bool :public Value
		{
		public:
			Bool(bool b) :value(b) {}
			std::string to_string()const override
			{
				if (value)
					return "true";
				return "false";
			}
			size_t get_type()const override
			{
				return BasicType::BOOL;
			}
			Object Convert(int type)override;
			Object get_negation()override
			{
				return std::make_shared<Bool>(!value);
			}
			Object operator==(Object v)override
			{
				return std::make_shared<Bool>(value == std::static_pointer_cast<Bool>(v->Convert(BOOL))->value);
			}
			Object operator!=(Object v)override
			{
				return std::make_shared<Bool>(value != std::static_pointer_cast<Bool>(v->Convert(BOOL))->value);
			}
			Object operator&& (Object v)override
			{
				return std::make_shared<Bool>(value && std::static_pointer_cast<Bool>(v->Convert(BOOL))->value);
			}
			Object operator||(Object v)override
			{
				return std::make_shared<Bool>(value || std::static_pointer_cast<Bool>(v->Convert(BOOL))->value);
			}
			Object clone()const override 
			{
				return std::make_shared<Bool>(value);
			}
			bool _value() { return value; }
		private:
			bool value;
		};
		class Int :public Value
		{
		public:
			Int(int v) :value(v) {}
			std::string to_string()const override
			{
				return std::to_string(value);
			}
			size_t get_type()const override
			{
				return BasicType::INT;
			}
			Object operator=(Object v)override
			{
				auto tmp = v->Convert(INT);
				value = std::static_pointer_cast<Int>(v)->value;
				return tmp;
			}
			Object operator+=(Object v)override
			{
				return std::make_shared<Int>(value += std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator-=(Object v)override
			{
				return std::make_shared<Int>(value -= std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator*=(Object v)override
			{
				return std::make_shared<Int>(value *= std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator/=(Object v)override
			{
				return std::make_shared<Int>(value /= std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator+ (Object v)override
			{
				return std::make_shared<Int>(value + std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator- (Object v)override
			{
				return std::make_shared<Int>(value -
					std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator* (Object v)override
			{
				return std::make_shared<Int>(value *
					std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator/ (Object v)override
			{
				return std::make_shared<Int>(value /
					std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator> (Object v)override
			{
				return std::make_shared < Bool >(value > std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator>= (Object v)override
			{
				return std::make_shared<Bool>(value >= std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator< (Object v)override
			{
				return std::make_shared<Bool>(value < std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator<= (Object v)override
			{
				return std::make_shared<Bool>(value <= std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator== (Object v)override
			{
				return std::make_shared<Bool>(value == std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object operator!= (Object v)override
			{
				return std::make_shared<Bool>(value != std::static_pointer_cast<Int>(v->Convert(INT))->value);
			}
			Object clone()const override
			{
				return std::make_shared<Int>(value);
			}
			Object get_negation()override
			{
				return std::make_shared<Int>(-value);
			}
			int get_value()
			{
				return value;
			}
			Object Convert(int type) override;
			Object operator[](Object v)override { throw Error("int doesn't have a member <operator[](int)>"); }
		private:
			int value;
		};
		class Double :public Value
		{
		public:
			Double(double v) :value(v) {}
			std::string to_string()const override
			{
				return std::to_string(value);
			}
			size_t get_type()const override
			{
				return BasicType::DOUBLE;
			}
			Object operator=(Object v)override;
			Object operator+=(Object v)override
			{
				return std::make_shared<Double>(value +=
					std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator-=(Object v)override
			{
				return std::make_shared<Double>(value -=
					std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator*=(Object v)override
			{
				return std::make_shared<Double>(value *=
					std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator/=(Object v)override
			{
				return std::make_shared<Double>(value /=
					std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator+(Object v)override
			{
				return std::make_shared<Double>(value +
					std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator-(Object v)override
			{
				return std::make_shared<Double>(value -
					std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator*(Object v)override
			{
				return std::make_shared<Double>(value *
					std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator/(Object v)override
			{
				return std::make_shared<Double>(value /
					std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator<(Object v)override
			{
				return std::make_shared<Bool>(value < std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator>(Object v)override
			{
				return std::make_shared<Bool>(value > std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator<=(Object v)override
			{
				return std::make_shared<Bool>(value <= std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator>=(Object v)override
			{
				return std::make_shared<Bool>(value >= std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator==(Object v)override
			{
				return std::make_shared<Bool>(value == std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object operator!=(Object v)override
			{
				return std::make_shared<Bool>(value != std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object clone()const override
			{
				return std::make_shared<Double>(value);
			}
			Object get_negation()override
			{
				return std::make_shared<Double>(-value);
			}
			Object operator[](Object v)override { throw Error("double doesn't have a member <operator[](int)>"); }
			double get_value()
			{
				return value;
			}
			Object Convert(int type)override;
		private:
			double value;
		};
		class String :public Value
		{
		public:
			String(const std::string &v) :str(v) {  }
			String(char ch) :str(std::string(1, ch)) {}
			Object operator+(Object v)override
			{
				return std::make_shared<String>(str +
					std::static_pointer_cast<String>(v->Convert(STRING))->str);
			}
			Object operator=(Object v)override
			{
				str = std::static_pointer_cast<String>(v->Convert(STRING))->str;
				return Convert(Mem::STRING);
			}
			size_t get_type()const override
			{
				return BasicType::STRING;
			}
			Object Convert(int type)override
			{
				if (type == STRING)
					return std::make_shared<String>(str);
				else
					throw Error("type-convert failed");
			}
			Object operator>(Object v)override
			{
				return std::make_shared<Bool>(str.size() > std::static_pointer_cast<String>(v->Convert(STRING))->str.size());
			}
			Object operator<(Object v)override
			{
				return std::make_shared<Bool>(str.size() < std::static_pointer_cast<String>(v->Convert(STRING))->str.size());
			}
			Object operator>=(Object v)override
			{
				return std::make_shared<Bool>(str.size() >= std::static_pointer_cast<String>(v->Convert(STRING))->str.size());
			}
			Object operator<=(Object v)override
			{
				return std::make_shared<Bool>(str.size() <= std::static_pointer_cast<String>(v->Convert(STRING))->str.size());
			}
			Object operator!=(Object v)override
			{
				return std::make_shared<Bool>(str.size() != std::static_pointer_cast<String>(v->Convert(STRING))->str.size());
			}
			Object operator==(Object v)override
			{
				return std::make_shared<Bool>(str.size() == std::static_pointer_cast<String>(v->Convert(STRING))->str.size());
			}
			Object operator[](Object v)override
			{
				return Object(new String(str[std::static_pointer_cast<Int>(v)->get_value()]));
			}
			Object clone()const override
			{
				return std::make_shared<String>(str);
			}
			std::string to_string()const override
			{
				return str;
			}		
			std::string str;
		};
		class ArrayObj :public Value
		{
		public:
			ArrayObj(std::vector<Object> && lst, size_t type_c) :elems(lst), type_code(type_c) {}
			ArrayObj(size_t sz, size_t type_c) : type_code(type_c),elems(sz) {}
			virtual Object operator=(Object v)
			{
				if (v->get_type() != BasicType::ARRAY&&std::static_pointer_cast<ArrayObj>(v)->get_ele_type() != type_code)
					throw Error("assign to an array failed.");
				elems = std::static_pointer_cast<ArrayObj>(v)->elems;
			}
			virtual Object operator[](Object v)
			{
				return elems[std::static_pointer_cast<Int>(v->Convert(INT))->get_value()];
			}
			virtual size_t get_type()const
			{
				return BasicType::ARRAY;
			}
			size_t get_ele_type()const
			{
				return type_code;
			}
		private:
			size_t type_code;
			std::vector<Object> elems;
		};
	}
}