#pragma once
#include <string>
#include <iostream>
#include "../../Lexer/HPP/Token.hpp"
namespace Simcc
{
	namespace Runtime
	{
		class ObjectBase {
		public:
			enum VType
			{
				Int = 1, Long = 2, Double = 3, Char = 4, String = 5, Bool = 6, User
			};
			enum CompareArg
			{
				Ne, Ge, Gt, Le, Lt, Eq,
			};
			ObjectBase(VType vt) :type(vt), data(nullptr) {}
			ObjectBase(int32_t a) :data(new int32_t(a)), type(Int) {}
			ObjectBase(double a) :data(new double(a)), type(Double) {}
			ObjectBase(char a) :data(new char(a)), type(Char) {}
			ObjectBase(std::string a) :data(new std::string(a)), type(String) {}
			ObjectBase(int64_t a) :data(new int64_t(a)), type(Long) {}
			ObjectBase(bool a) :data(new bool(a)), type(Bool) {}
			ObjectBase(const ObjectBase& v)
			{
				switch (v.type)
				{
				case Int:
					data = new int32_t(*(int32_t*)v.data);
					type = Int;
					break;
				case Double:
					data = new double(*(double*)v.data);
					type = Double;
					break;
				case Long:
					data = new int64_t(*(int64_t*)v.data);
					type = Long;
					break;
				case Char:
					data = new char(*(char*)v.data);
					type = Char;
					break;
				case String:
					data = new std::string(*(std::string*)v.data);
					type = String;
					break;
				case Bool:
					data = new bool(*(bool*)v.data);
					type = Bool;
					break;
				default:
					std::cout << "ERROR:" << v.type << "  ";
					throw std::runtime_error("Simcc::Runtime::ObjectBase(const ObjectBase&) 1");
					break;
				}
			}
			virtual ObjectBase& operator=(const ObjectBase&v)
			{
				if (v.data == data)
					return *this;
				delete data;
				switch (v.type)
				{
				case Int:
					data = new int32_t(*(int32_t*)v.data);
					type = Int;
					break;
				case Double:
					data = new double(*(double*)v.data);
					type = Double;
					break;
				case Long:
					data = new int64_t(*(int64_t*)v.data);
					type = Long;
					break;
				case Char:
					data = new char(*(char*)v.data);
					type = Char;
					break;
				case String:
					data = new std::string(*(std::string*)v.data);
					type = String;
					break;
				case Bool:
					data = new bool(*(bool*)v.data);
					type = Bool;
					break;
				default:
					throw std::runtime_error("Simcc::Runtime::ObjectBase(const ObjectBase&) 1");
					break;
				}
				return *this;
			}
			virtual ObjectBase& operator++()
			{
				switch (type)
				{
				case Simcc::Runtime::ObjectBase::Double:
					*(double*)data += 1;
					break;
				case Simcc::Runtime::ObjectBase::Long:
					*(int64_t*)data += 1;
					break;
				case Simcc::Runtime::ObjectBase::Int:
					*(int32_t*)data += 1;
					break;
				case Simcc::Runtime::ObjectBase::Char:
					*(char*)data += 1;
					break;
				default:
					throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator+(const ObjectBase & v) const ");
				}
				return *this;
			}
			virtual ObjectBase& operator--()
			{
				switch (type)
				{
				case Simcc::Runtime::ObjectBase::Double:
					*(double*)data -= 1;
					break;
				case Simcc::Runtime::ObjectBase::Long:
					*(int64_t*)data -= 1;
					break;
				case Simcc::Runtime::ObjectBase::Int:
					*(int32_t*)data -= 1;
					break;
				case Simcc::Runtime::ObjectBase::Char:
					*(char*)data -= 1;
					break;
				default:
					throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator+(const ObjectBase & v) const ");
				}
				return *this;
			}
			virtual ~ObjectBase() {
				if (data != nullptr)
					delete data;
			}
			template <typename T>
			T& get_value()const
			{
				return *(T*)data;
			}
			int32_t to_int()const;
			double to_double()const;
			int64_t to_long()const;
			bool to_bool()const;
			virtual std::string to_string()const;
			//=========================== 初始化和转换====================
			virtual void SelfAdd(const ObjectBase *obj1)
			{
				switch (type)
				{
				case Simcc::Runtime::ObjectBase::Double:
					*(double*)data = *(double*)data + obj1->to_double();
					break;
				case Simcc::Runtime::ObjectBase::Long:
					*(int64_t*)data = *(int64_t*)data + obj1->to_long();
					break;
				case Simcc::Runtime::ObjectBase::Int:
					*(int32_t*)data = *(int32_t*)data + obj1->to_int();
					break;
				case Simcc::Runtime::ObjectBase::Char:
					*(char*)data = *(char*)data + obj1->to_int();
					break;
				case Simcc::Runtime::ObjectBase::String:
					*(std::string*)data = *(std::string*)data + *(std::string*)obj1->data;
					break;
				default:
					throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator+(const ObjectBase & v) const ");
				}
			}
			virtual void SelfSub(const ObjectBase *obj1)
			{
				switch (type)
				{
				case Simcc::Runtime::ObjectBase::Double:
					*(double*)data = *(double*)data - obj1->to_double();
					break;
				case Simcc::Runtime::ObjectBase::Long:
					*(int64_t*)data = *(int64_t*)data - obj1->to_long();
					break;
				case Simcc::Runtime::ObjectBase::Int:
					*(int32_t*)data = *(int32_t*)data - obj1->to_int();
					break;
				case Simcc::Runtime::ObjectBase::Char:
					*(char*)data = *(char*)data - obj1->to_int();
					break;
				default:
					throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator+(const ObjectBase & v) const ");
				}
			}
			virtual void SelfMul(const ObjectBase *obj1)
			{
				switch (type)
				{
				case Simcc::Runtime::ObjectBase::Double:
					*(double*)data = *(double*)data * obj1->to_double();
					break;
				case Simcc::Runtime::ObjectBase::Long:
					*(int64_t*)data = *(int64_t*)data * obj1->to_long();
					break;
				case Simcc::Runtime::ObjectBase::Int:
					*(int32_t*)data = *(int32_t*)data * obj1->to_int();
					break;
				case Simcc::Runtime::ObjectBase::Char:
					*(char*)data = *(char*)data * obj1->to_int();
					break;
				default:
					throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator+(const ObjectBase & v) const ");
				}
			}
			virtual void SelfDiv(const ObjectBase *obj1)
			{
				switch (type)
				{
				case Simcc::Runtime::ObjectBase::Double:
					*(double*)data = *(double*)data / obj1->to_double();
					break;
				case Simcc::Runtime::ObjectBase::Long:
					*(int64_t*)data = *(int64_t*)data / obj1->to_long();
					break;
				case Simcc::Runtime::ObjectBase::Int:
					*(int32_t*)data = *(int32_t*)data / obj1->to_int();
					break;
				case Simcc::Runtime::ObjectBase::Char:
					*(char*)data = *(char*)data / obj1->to_int();
					break;
				default:
					throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator+(const ObjectBase & v) const ");
				}
			}
			virtual ObjectBase* Add(const ObjectBase *obj1);// 这样玩指针 吃枣内存泄露, 后来慢慢查把．<{=．．．． 
			virtual ObjectBase* Sub(const ObjectBase *obj1);
			virtual ObjectBase* Mul(const ObjectBase *obj1);
			virtual ObjectBase* Div(const ObjectBase *obj1);
			virtual ObjectBase* Compare(const ObjectBase *obj, Simcc::Lexer::Tag t);
			VType type;
		private:
			//=================== 这些都不露面啦=====================
			ObjectBase operator+ (const ObjectBase&v)const;
			ObjectBase operator- (const ObjectBase&v)const;
			ObjectBase operator*(const ObjectBase&v)const;
			ObjectBase operator/(const ObjectBase&v)const;
			ObjectBase operator==(const ObjectBase&v)const;
			ObjectBase operator>=(const ObjectBase&v)const;
			ObjectBase operator<=(const ObjectBase&v)const;
			ObjectBase operator!=(const ObjectBase&v)const;
			ObjectBase operator>(const ObjectBase&v)const;
			ObjectBase operator<(const ObjectBase&v)const;
			void *data;

		};
	}
	namespace Test
	{
		struct pos
		{
			pos(int32_t a, int32_t b) :x(a), y(b) {}
			int32_t x;
			int32_t y;
		};
		void object_test();
	}
}
