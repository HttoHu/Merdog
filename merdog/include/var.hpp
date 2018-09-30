#pragma once
#include <string>
#include <memory>
#include <set>
#include <map>
#include "error.hpp"
namespace Mer
{
	class Namespace;
	class Token;
	namespace Mem
	{
		enum BasicType
		{
			NDEF = -1, BVOID = 0, INT = 1, DOUBLE = 3, STRING = 5, BOOL = 7
		};
		class Type;
		class Value;
		using Object = std::shared_ptr<Value>;
		extern std::map<size_t, Type*> type_map;
		extern int type_counter;
		extern Namespace *this_namespace;

		void set_new_type(const std::string & type_name, Type *t);
		std::string type_to_string(BasicType bt);
		size_t get_type_code(Token *tok);
		size_t &type_no();

		class Type
		{
		public:
			Type(const std::string &_name, BasicType bt, const std::set<size_t>& _convertible_types)
				:name(_name), type_code(bt), convertible_types(_convertible_types) {}
			bool convertible(const size_t &t);
			virtual std::string to_string() { return "{TYPE:}"+name+"/n"; }
			virtual ~Type() {}

		private:
			std::set<size_t> convertible_types;
			BasicType type_code;
			std::string name;
		};

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
			Int(int64_t v) :value(v) {}
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
			int64_t get_value()
			{
				return value;
			}
			Object Convert(int type) override;
			Object operator[](Object v)override { throw Error("int doesn't have a member <operator[](int)>"); }
		private:
			int64_t value;
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
			Object clone()const override
			{
				return std::make_shared<String>(str);
			}
			std::string to_string()const override
			{
				return str;
			}
		private:
			std::string str;
		};
	}
}