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
			INT,DOUBLE,STRING,BOOL
		};
		class Value;
		std::string type_to_string(BasicType bt);
		using Object = std::shared_ptr<Value>;
		class Value
		{
		public:
			virtual std::string to_string()const { return ""; }
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
			virtual Object  Convert(int type) { throw Error("syntax error"); }
			virtual Object  get_negation()
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
			Object Convert(int type)override;
			Object get_negation()override
			{
				return std::make_shared<Bool>(!value);
			}
			Object operator==(Object v)
			{
				return std::make_shared<Bool>(value == std::static_pointer_cast<Bool>(v->Convert(BOOL))->value);
			}
			Object operator!=(Object v)
			{
				return std::make_shared<Bool>(value != std::static_pointer_cast<Bool>(v->Convert(BOOL))->value);
			}
			Object operator&& (Object v)
			{
				return std::make_shared<Bool>(value && std::static_pointer_cast<Bool>(v->Convert(BOOL))->value);
			}
			Object operator||(Object v)
			{
				return std::make_shared<Bool>(value || std::static_pointer_cast<Bool>(v->Convert(BOOL))->value);
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
				return std::make_shared < Bool > (value > std::static_pointer_cast<Int>(v->Convert(INT))->value);
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
			Object get_negation()override
			{
				return std::make_shared<Int>(-value);
			}
			Object Convert(int type) override;
			Object operator[](Object v) { throw Error("int doesn't have a member <operator[](int)>"); }
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
			Object operator=(Object v)override
			{
				value = std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value;
				return Object(this);
			}
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
				return std::make_shared<Bool>(value!=std::static_pointer_cast<Double>(v->Convert(DOUBLE))->value);
			}
			Object get_negation()override
			{
				return std::make_shared<Double>(-value);
			}
			Object operator[](Object v) { throw Error("double doesn't have a member <operator[](int)>"); }

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
					std::static_pointer_cast<String>(v->Convert(INT))->str);
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

			std::string to_string()const override
			{
				return str;
			}
		private:
			std::string str;
		};
		class Ref :public Value
		{
		public:
			Ref(size_t p) :pos(p)
			{
			}
			Object operator=(Object obj)override;
			Object operator+(Object obj)override;
			Object operator-(Object obj)override;
			Object operator*(Object obj)override;
			Object operator/(Object obj)override;
			Object operator+=(Object obj)override;
			Object operator-=(Object obj)override;
			Object operator*=(Object obj)override;
			Object operator/=(Object obj)override;
			Object operator==(Object obj)override;
			Object operator>=(Object obj)override;
			Object operator<=(Object obj)override;
			Object operator>(Object obj)override;
			Object operator<(Object obj)override;
			Object operator!=(Object obj)override;
			Object operator&&(Object obj)override;
			Object operator||(Object obj)override;
			Object operator[](Object obj)override;
			Object  Convert(int type)override;
			Object  get_negation()override;
			std::string to_string()const override;
		private:
			size_t pos;
		};
	}
}