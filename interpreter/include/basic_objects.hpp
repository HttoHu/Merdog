/*
		MIT License

		Copyright (c) 2019 HttoHu

		Permission is hereby granted, free of charge, to any person obtaining a copy
		of this software and associated documentation files (the "Software"), to deal
		in the Software without restriction, including without limitation the rights
		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the Software is
		furnished to do so, subject to the following conditions:

		The above copyright notice and this permission notice shall be included in all
		copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
		SOFTWARE.

*   note that : obj refers to the instance of Mem::Int or Mem::String, Mem::Bool etc.
*	The Object is a shared_ptr<Value> which is the parent of all objs.
*	special usage: When we need to get the raw of a Mem::xxx (e.g. Mem::Int) we can use get_raw<int>(obj) ),
*	I supposed that the obj memory structure is like that
*	| --- virtual_table_pointer(or some other value about parent : x64(8Byte), x86(4Byte) ---|
*	| int or string , bool, real according the obj type																|
*	so move 8/4 Byte could get the raw value of the obj.
*/
#pragma once
#include <string>
#include <set>
#include <map>
#include <memory>
#include <vector>
#include "error.hpp"
#include "type.hpp"
#include "clib/any.hpp"
using type_code_index = int;
const int BASICTYPE_MAX_CODE = 11;
namespace Mer
{
	class StructureBase;
	class Namespace;
	class Token;
	std::string type_to_string(size_t type_code);
	namespace Mem
	{

		class Type;
		class Value;

		extern Namespace* this_namespace;
		using Object = std::shared_ptr<Value>;
		void swap(Object rhs, Object lhs);
		class Value
		{
		public:
			virtual Object clone()const { return nullptr; }
			virtual std::string to_string()const { return ""; }
			virtual type_code_index get_type()const
			{
				return BasicType::NDEF;
			}
			virtual Object operator=(Object v)
			{
				return nullptr;
			}
			virtual Object operator+=(Object v)
			{
				throw Error("+=: syntax error");
			}
			virtual Object operator-=(Object v)
			{
				throw Error("-=: syntax error");
			}
			virtual Object operator*=(Object v)
			{
				throw Error("*=: syntax error");
			}
			virtual Object operator/=(Object v)
			{
				throw Error("/=: syntax error");
			}
			virtual Object operator+ (Object v) { throw Error("+: syntax error"); }
			virtual Object operator- (Object v) { throw Error("-: syntax error"); }
			virtual Object operator* (Object v) { throw Error("* syntax error"); }
			virtual Object operator/ (Object v) { throw Error("/: syntax error"); }
			virtual Object Convert(type_code_index type);
			virtual Object get_negation()
			{
				throw Error("get_negation: syntax error");
			}
			virtual Object operator>(Object v) { throw Error(">: syntax error"); }
			virtual Object operator<(Object v) { throw Error("<: syntax error"); }
			virtual Object operator>=(Object v) { throw Error(">=: syntax error"); }
			virtual Object operator<=(Object v) { throw Error("<=: syntax error"); }
			virtual Object operator !=(Object v) { throw Error("!=: syntax error"); }
			virtual Object operator ==(Object v) { throw Error("==: syntax error"); }
			virtual Object operator[](Object v) { throw Error("[]: syntax error"); }
			virtual Object operator&& (Object v) { throw Error("&&: syntax error"); }
			virtual Object operator|| (Object v) { throw Error("OR: syntax error"); }
			virtual ~Value() { }
		private:
		};
		std::string type_to_string(BasicType bt);
		//get type

		Mem::Object create_var_t(type_code_index type);
		// every obj extends Value, and override some common operator
		// size_t get_type() to get the obj's type
		// clone , copy the obj and return it;
		// Convert(BasicType), to convert the Obj to another compatible type obj.
		class Int;
		class Bool :public Value
		{
		public:
			Bool(bool b) :value(b) {}
			Object operator=(Object v)override
			{
				auto tmp = v;
				value = std::static_pointer_cast<Bool>(v)->value;
				return tmp;
			}
			std::string to_string()const override
			{
				if (value)
					return "true";
				return "false";
			}
			type_code_index get_type()const override
			{
				return BasicType::BOOL;
			}
			Object Convert(type_code_index type)override;
			Object get_negation()override
			{
				return std::make_shared<Mem::Bool>(!value);
			}
			Object operator==(Object v)override
			{
				return std::make_shared<Mem::Bool>(value == std::static_pointer_cast<Bool>(v)->value);
			}
			Object operator!=(Object v)override
			{
				return std::make_shared<Mem::Bool>(value != std::static_pointer_cast<Bool>(v)->value);
			}
			Object operator&& (Object v)override
			{
				return std::make_shared<Mem::Bool>(value && std::static_pointer_cast<Bool>(v)->value);
			}
			Object operator||(Object v)override
			{
				return std::make_shared<Mem::Bool>(value || std::static_pointer_cast<Bool>(v)->value);
			}
			Object clone()const override
			{
				return std::make_shared<Mem::Bool>(value);
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
			type_code_index get_type()const override
			{
				return BasicType::INT;
			}
			Object operator=(Object v)override;
			Object operator+=(Object v)override
			{
				return std::make_shared<Int>(value += std::static_pointer_cast<Int>(v)->value);
			}
			Object operator-=(Object v)override
			{
				return std::make_shared<Int>(value -= std::static_pointer_cast<Int>(v)->value);
			}
			Object operator*=(Object v)override
			{
				return std::make_shared<Int>(value *= std::static_pointer_cast<Int>(v)->value);
			}
			Object operator/=(Object v)override
			{
				return std::make_shared<Int>(value /= std::static_pointer_cast<Int>(v)->value);
			}
			Object operator+ (Object v)override
			{
				return std::make_shared<Int>(value + std::static_pointer_cast<Int>(v)->value);
			}
			Object operator- (Object v)override
			{
				return std::make_shared<Int>(value -
					std::static_pointer_cast<Int>(v)->value);
			}
			Object operator* (Object v)override
			{
				return std::make_shared<Int>(value *
					std::static_pointer_cast<Int>(v)->value);
			}
			Object operator/ (Object v)override
			{
				return std::make_shared<Int>(value /
					std::static_pointer_cast<Int>(v)->value);
			}
			Object operator> (Object v)override
			{
				return std::make_shared < Bool >(value > std::static_pointer_cast<Int>(v)->value);
			}
			Object operator>= (Object v)override
			{
				return std::make_shared<Bool>(value >= std::static_pointer_cast<Int>(v)->value);
			}
			Object operator< (Object v)override
			{
				return std::make_shared<Bool>(value < std::static_pointer_cast<Int>(v)->value);
			}
			Object operator<= (Object v)override
			{
				return std::make_shared<Bool>(value <= std::static_pointer_cast<Int>(v)->value);
			}
			Object operator== (Object v)override
			{
				return std::make_shared<Bool>(value == std::static_pointer_cast<Int>(v)->value);
			}
			Object operator!= (Object v)override
			{
				return std::make_shared<Bool>(value != std::static_pointer_cast<Int>(v)->value);
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
			static int get_val(Mem::Object obj) {
				return std::static_pointer_cast<Mem::Int>(obj)->get_value();
			}
			Object Convert(type_code_index type) override;
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
			type_code_index get_type()const override
			{
				return BasicType::DOUBLE;
			}
			Object operator=(Object v)override;
			Object operator+=(Object v)override
			{
				return std::make_shared<Double>(value +=
					std::static_pointer_cast<Double>(v)->value);
			}
			Object operator-=(Object v)override
			{
				return std::make_shared<Double>(value -=
					std::static_pointer_cast<Double>(v)->value);
			}
			Object operator*=(Object v)override
			{
				return std::make_shared<Double>(value *=
					std::static_pointer_cast<Double>(v)->value);
			}
			Object operator/=(Object v)override
			{
				return std::make_shared<Double>(value /=
					std::static_pointer_cast<Double>(v)->value);
			}
			Object operator+(Object v)override
			{
				return std::make_shared<Double>(value +
					std::static_pointer_cast<Double>(v)->value);
			}
			Object operator-(Object v)override
			{
				return std::make_shared<Double>(value -
					std::static_pointer_cast<Double>(v)->value);
			}
			Object operator*(Object v)override
			{
				return std::make_shared<Double>(value *
					std::static_pointer_cast<Double>(v)->value);
			}
			Object operator/(Object v)override
			{
				return std::make_shared<Double>(value /
					std::static_pointer_cast<Double>(v)->value);
			}
			Object operator<(Object v)override
			{
				return std::make_shared<Bool>(value < std::static_pointer_cast<Double>(v)->value);
			}
			Object operator>(Object v)override
			{
				return std::make_shared<Bool>(value > std::static_pointer_cast<Double>(v)->value);
			}
			Object operator<=(Object v)override
			{
				return std::make_shared<Bool>(value <= std::static_pointer_cast<Double>(v)->value);
			}
			Object operator>=(Object v)override
			{
				return std::make_shared<Bool>(value >= std::static_pointer_cast<Double>(v)->value);
			}
			Object operator==(Object v)override
			{
				return std::make_shared<Bool>(value == std::static_pointer_cast<Double>(v)->value);
			}
			Object operator!=(Object v)override
			{
				return std::make_shared<Bool>(value != std::static_pointer_cast<Double>(v)->value);
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
			Object Convert(type_code_index type)override;
		private:
			double value;
		};
		class String :public Value
		{
		public:
			String(const std::string& v) :str(v) {  }
			String(char ch) :str(std::string(1, ch)) {}
			Object operator+(Object v)override
			{
				return std::make_shared<String>(str +
					std::static_pointer_cast<String>(v)->str);
			}
			Object operator=(Object v)override
			{
				str = std::static_pointer_cast<String>(v)->str;
				return std::make_shared<String>(str);
			}
			type_code_index get_type()const override
			{
				return BasicType::STRING;
			}
			Object Convert(type_code_index type)override
			{
				return std::make_shared<String>(str);
			}
			Object operator+=(Object v)override
			{
				return std::make_shared<String>(str += std::static_pointer_cast<String>(v)->str);
			}
			Object operator>(Object v)override
			{
				return std::make_shared<Bool>(str > std::static_pointer_cast<String>(v)->str);
			}
			Object operator<(Object v)override
			{
				return std::make_shared<Bool>(str < std::static_pointer_cast<String>(v)->str);
			}
			Object operator>=(Object v)override
			{
				return std::make_shared<Bool>(str >= std::static_pointer_cast<String>(v)->str);
			}
			Object operator<=(Object v)override
			{
				return std::make_shared<Bool>(str <= std::static_pointer_cast<String>(v)->str);
			}
			Object operator!=(Object v)override
			{
				return std::make_shared<Bool>(str != std::static_pointer_cast<String>(v)->str);
			}
			Object operator==(Object v)override
			{
				return std::make_shared<Bool>(str == std::static_pointer_cast<String>(v)->str);
			}
			Object operator[](Object v)override;
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
		class InitListObj :public Value
		{
		public:
			InitListObj(std::vector<Object>&& lst, type_code_index type_c) :elems(lst), type_code(type_c) {}
			InitListObj(size_t sz, type_code_index type_c) : type_code(type_c), elems(sz) {}
			virtual Object operator[](Object v)override
			{
				return elems[std::static_pointer_cast<Int>(v)->get_value()];
			}
			virtual type_code_index get_type()const override
			{
				return BasicType::ARRAY;
			}
			type_code_index get_ele_type()const
			{
				return type_code;
			}
			Mem::Object clone()const override;
			type_code_index type_code;
			std::vector<Object> elems;
		};
		class Pointer :public Value
		{
		public:
			Pointer(size_t s) = delete;
			Pointer(Object _obj);
			Mem::Object operator=(Object v)override;
			Mem::Object operator==(Object v)override;
			Mem::Object operator!=(Object v)override;
			Mem::Object clone()const override;
			Mem::Object rm_ref() { return obj; }
			Object operator[](Object v)override;
			std::string to_string()const override
			{
				return "pointer " + obj->to_string();
			}
			~Pointer();

		private:
			Object obj;
		};
		class Char :public Value
		{
		public:
			Char(char v) :value(new char(v)) {}
			Char(char* v) :value(v), del(false) {}
			std::string to_string()const override
			{
				return std::string(1, *value);
			}
			type_code_index get_type()const override
			{
				return BasicType::CHAR;
			}
			Object operator=(Object v)override;
			Object operator+=(Object v)override
			{
				return std::make_shared<Char>(*value += *std::static_pointer_cast<Char>(v)->value);
			}
			Object operator-=(Object v)override
			{
				return std::make_shared<Char>(*value -= *std::static_pointer_cast<Char>(v)->value);
			}
			Object operator*=(Object v)override
			{
				return std::make_shared<Char>(*value *= *std::static_pointer_cast<Char>(v)->value);
			}
			Object operator/=(Object v)override
			{
				return std::make_shared<Char>(*value /= *std::static_pointer_cast<Char>(v)->value);
			}
			Object operator+ (Object v)override
			{
				return std::make_shared<Char>(*value + *std::static_pointer_cast<Char>(v)->value);
			}
			Object operator- (Object v)override
			{
				return std::make_shared<Char>(*value -
					*std::static_pointer_cast<Char>(v)->value);
			}
			Object operator* (Object v)override
			{
				return std::make_shared<Char>(*value *
					*std::static_pointer_cast<Char>(v)->value);
			}
			Object operator/ (Object v)override
			{
				return std::make_shared<Char>(*value /
					*std::static_pointer_cast<Char>(v)->value);
			}
			Object operator> (Object v)override
			{
				return std::make_shared < Bool >(*value > * std::static_pointer_cast<Char>(v)->value);
			}
			Object operator>= (Object v)override
			{
				return std::make_shared<Bool>(*value >= *std::static_pointer_cast<Char>(v)->value);
			}
			Object operator< (Object v)override
			{
				return std::make_shared<Bool>(*value < *std::static_pointer_cast<Char>(v)->value);
			}
			Object operator<= (Object v)override
			{
				return std::make_shared<Bool>(*value <= *std::static_pointer_cast<Char>(v)->value);
			}
			Object operator== (Object v)override
			{
				return std::make_shared<Bool>(*value == *std::static_pointer_cast<Char>(v)->value);
			}
			Object operator!= (Object v)override
			{
				return std::make_shared<Bool>(*value != *std::static_pointer_cast<Char>(v)->value);
			}
			Object clone()const override
			{
				return std::make_shared<Char>(*value);
			}
			Object get_negation()override
			{
				return std::make_shared<Char>(-*value);
			}
			char get_value()
			{
				return *value;
			}
			Object Convert(type_code_index type) override;
			Object operator[](Object v)override { throw Error("char doesn't have a member <operator[](int)>"); }
			virtual ~Char() { if (del)delete value; }
		private:
			bool del = true;
			char* value;
		};
		class AnyObj :public Mem::Value {
		public:
			template<typename T>
			AnyObj(const T& t) :obj(t) {}
			template<typename T>
			AnyObj& operator=(const T& t)
			{
				obj = t;
				return *this;
			}
			template<typename T>
			AnyObj& operator==(T&& t)noexcept {
				obj = t;
			}
			template<typename T>
			T& cast() {
				return mer::any_cast<T&>(obj);
			}
			Mem::Object clone()const override;
		private:
			mer::any obj;
		};
		class Array :public Value
		{
		public:
			Array(type_code_index _type, int _pos, int _length) :type(_type), pos(_pos), length(_length) {}
			Mem::Object operator[](Object index)override;
			Mem::Object clone()const override;
			type_code_index get_type()const override { return type; }
		private:
			// element type
			type_code_index type;
			int pos;
			int length;
		};
		class GArray :public Value
		{
		public:
			GArray(type_code_index _type, int _pos, int _length) :type(_type), pos(_pos), length(_length) {}
			Mem::Object operator[](Object index)override;
			Mem::Object clone()const override;
			type_code_index get_type()const override { return type; }
		private:
			// element type
			type_code_index type;
			int pos;
			int length;
		};
		template<typename T>
		T get_raw(Object obj)
		{
			// An UB operation to improve performance, but just be careful, I've tested in Microsoft CL, Clang ,GCC.
#ifndef SAFE_MOD
			return *(T*)((char*)obj.get() + sizeof(void*));
#else
			throw Error("unsafe calling");
#endif
		}

	}
}