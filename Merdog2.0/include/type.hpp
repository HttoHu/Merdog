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
			INT,DOUBLE,STRIHG,
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
			virtual ~Value() {}
		private:
		};
		using Raw = std::shared_ptr<Value>;
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

			Value *Convert(int type)override;
		private:
			double value;
		};
	}
}