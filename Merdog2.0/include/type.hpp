#pragma once
#include <string>
#include <memory>
#include "error.hpp"
namespace Mer
{
	namespace Mem
	{
		enum BasicType
		{
			INT,DOUBLE,STRING,
		};
		std::string type_to_string(BasicType bt);
		class Value
		{
		public:
			virtual std::string to_string()const { return ""; }
			virtual Value* operator=(Value *v)
			{
				return nullptr;
			}
			virtual Value* operator+ (Value *v) { return nullptr; }
			virtual Value* operator- (Value *v) { return nullptr; }
			virtual Value* operator* (Value *v) { return nullptr; }
			virtual Value* operator/ (Value *v) { return nullptr; }
			virtual Value * Convert(int type) { return nullptr; }
			virtual Value * get_negation()
			{
				return nullptr;
			}
			virtual Value *operator[](Value *v) { return nullptr; }
			virtual ~Value() {}
		private:
		};
		using Object = std::shared_ptr<Value>;
		class Int :public Value
		{
		public:
			Int(int64_t v) :value(v) {}
			std::string to_string()const override
			{
				return std::to_string(value);
			}
			Value* operator=(Value *v)override
			{
				auto tmp = v->Convert(INT);
				value = static_cast<Int*>(v)->value;
				return tmp;
			}
			Value *operator+ (Value *v)override
			{
				auto tmp = v->Convert(INT);
				auto ret= new Int(value + static_cast<Int*>(tmp)->value);
				delete tmp;
				return ret;
			}
			Value *operator- (Value *v)override
			{
				auto tmp = v->Convert(INT);
				auto ret = new Int(value - static_cast<Int*>(tmp)->value);
				delete tmp;
				return ret;
			}
			Value *operator* (Value *v)override
			{
				auto tmp = v->Convert(INT);
				auto ret = new Int(value * static_cast<Int*>(tmp)->value);
				delete tmp;
				return ret;
			}
			Value *operator/ (Value *v)override
			{
				auto tmp = v->Convert(INT);
				auto ret = new Int(value / static_cast<Int*>(tmp)->value);
				delete tmp;
				return ret;
			}
			Value *get_negation()override
			{
				return new Int(-value);
			}
			Value *Convert(int type) override;
			Value *operator[](Value *v) { throw Error("int doesn't have a member <operator[](int)>"); }
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
			Value* operator=(Value *v)override
			{
				auto tmp = v->Convert(DOUBLE);
				value = static_cast<Double*>(v)->value;
				return tmp;
			}

			Value *operator+(Value *v)override
			{
				auto tmp = v->Convert(DOUBLE);
				auto ret= new Double(value + static_cast<Double*>(tmp)->value);
				delete tmp;
				return ret;
			}
			Value *operator-(Value *v)override
			{
				auto tmp = v->Convert(DOUBLE);
				auto ret = new Double(value - static_cast<Double*>(tmp)->value);
				delete tmp;
				return ret;
			}
			Value *operator*(Value *v)override
			{
				auto tmp = v->Convert(DOUBLE);
				auto ret = new Double(value * static_cast<Double*>(tmp)->value);
				delete tmp;
				return ret;
			}
			Value *operator/(Value *v)override
			{
				auto tmp = v->Convert(DOUBLE);
				auto ret = new Double(value / static_cast<Double*>(tmp)->value);
				delete tmp;
				return ret;
			}
			Value *get_negation()override
			{
				return new Double(-value);
			}
			Value *operator[](Value *v) { throw Error("double doesn't have a member <operator[](int)>"); }

			Value *Convert(int type)override;
		private:
			double value;
		};
		class String :public Value
		{
		public:
			String(const std::string &v) :str(v) {  }
			String(char ch) :str(std::string(1, ch)) {}
			Value *operator+(Value *v)override
			{
				auto tmp = static_cast<String*>(v->Convert(STRING));
				auto ret= new String(str + tmp->str);
				delete tmp;
				return ret;
			}
			Value *Convert(int type)override
			{
				if (type == STRING)
					return new String(str);
				else
					throw Error("type-convert failed");
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