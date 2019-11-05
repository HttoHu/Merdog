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
#include <any>
#include <string>
#include <memory>
#include <set>
#include <map>
#include <vector>
#include "type.hpp"
#include "error.hpp"
#define BASICTYPE_MAX_CODE 9
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
		using Object = std::shared_ptr<Value>;

		extern Namespace* this_namespace;

		std::string type_to_string(BasicType bt);
		//get type
		size_t get_type_code();
		size_t get_type_code(Token* tok);
		size_t& type_no();
		// to get a compound type's code like vector<map<int,real>>
		size_t get_ctype_code();
		size_t merge_two_type(size_t c, size_t e);
		Mem::Object create_var_t(size_t type);
		// every obj extends Value, and override some common operator
		// size_t get_type() to get the obj's type
		// clone , copy the obj and return it;
		// Convert(BasicType), to convert the Obj to another compatible type obj.
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
			virtual Object Convert(size_t type) { throw Error("convert: syntax error"); }
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
			size_t get_type()const override
			{
				return BasicType::BOOL;
			}
			Object Convert(size_t type)override;
			Object get_negation()override
			{
				return std::make_shared<Bool>(!value);
			}
			Object operator==(Object v)override
			{
				return std::make_shared<Bool>(value == std::static_pointer_cast<Bool>(v)->value);
			}
			Object operator!=(Object v)override
			{
				return std::make_shared<Bool>(value != std::static_pointer_cast<Bool>(v)->value);
			}
			Object operator&& (Object v)override
			{
				return std::make_shared<Bool>(value && std::static_pointer_cast<Bool>(v)->value);
			}
			Object operator||(Object v)override
			{
				return std::make_shared<Bool>(value || std::static_pointer_cast<Bool>(v)->value);
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
			static int get_val(Mem::Object obj){
				return std::static_pointer_cast<Mem::Int>(obj)->get_value();
			}
			Object Convert(size_t type) override;
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
			Object Convert(size_t type)override;
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
			size_t get_type()const override
			{
				return BasicType::STRING;
			}
			Object Convert(size_t type)override
			{
				return std::make_shared<String>(str);
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
			InitListObj(std::vector<Object>&& lst, size_t type_c) :elems(lst), type_code(type_c) {}
			InitListObj(size_t sz, size_t type_c) : type_code(type_c), elems(sz) {}
			virtual Object operator[](Object v)
			{
				return elems[std::static_pointer_cast<Int>(v)->get_value()];
			}
			virtual size_t get_type()const
			{
				return BasicType::ARRAY;
			}
			size_t get_ele_type()const
			{
				return type_code;
			}

			size_t type_code;
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
				return "pointer "+obj->to_string();
			}
			~Pointer();

		private:
			Object obj;
		};
		class Char :public Value
		{
		public:
			Char(char v) :value(v) {}
			std::string to_string()const override
			{
				return std::string(1, value);
			}
			size_t get_type()const override
			{
				return BasicType::CHAR;
			}
			Object operator=(Object v)override;
			Object operator+=(Object v)override
			{
				return std::make_shared<Int>(value += std::static_pointer_cast<Char>(v)->value);
			}
			Object operator-=(Object v)override
			{
				return std::make_shared<Int>(value -= std::static_pointer_cast<Char>(v)->value);
			}
			Object operator*=(Object v)override
			{
				return std::make_shared<Int>(value *= std::static_pointer_cast<Char>(v)->value);
			}
			Object operator/=(Object v)override
			{
				return std::make_shared<Int>(value /= std::static_pointer_cast<Char>(v)->value);
			}
			Object operator+ (Object v)override
			{
				return std::make_shared<Int>(value + std::static_pointer_cast<Char>(v)->value);
			}
			Object operator- (Object v)override
			{
				return std::make_shared<Char>(value -
					std::static_pointer_cast<Char>(v)->value);
			}
			Object operator* (Object v)override
			{
				return std::make_shared<Char>(value *
					std::static_pointer_cast<Char>(v)->value);
			}
			Object operator/ (Object v)override
			{
				return std::make_shared<Char>(value /
					std::static_pointer_cast<Char>(v)->value);
			}
			Object operator> (Object v)override
			{
				return std::make_shared < Bool >(value > std::static_pointer_cast<Char>(v)->value);
			}
			Object operator>= (Object v)override
			{
				return std::make_shared<Bool>(value >= std::static_pointer_cast<Char>(v)->value);
			}
			Object operator< (Object v)override
			{
				return std::make_shared<Bool>(value < std::static_pointer_cast<Char>(v)->value);
			}
			Object operator<= (Object v)override
			{
				return std::make_shared<Bool>(value <= std::static_pointer_cast<Char>(v)->value);
			}
			Object operator== (Object v)override
			{
				return std::make_shared<Bool>(value == std::static_pointer_cast<Char>(v)->value);
			}
			Object operator!= (Object v)override
			{
				return std::make_shared<Bool>(value != std::static_pointer_cast<Char>(v)->value);
			}
			Object clone()const override
			{
				return std::make_shared<Char>(value);
			}
			Object get_negation()override
			{
				return std::make_shared<Char>(-value);
			}
			int get_value()
			{
				return value;
			}
			Object Convert(size_t type) override;
			Object operator[](Object v)override { throw Error("int doesn't have a member <operator[](int)>"); }
		private:
			int value;
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
				return std::any_cast<T&>(obj);
			}
			Mem::Object clone()const override;
		private:
			std::any obj;
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