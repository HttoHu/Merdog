#pragma once
#include <iostream>
#include "lexer.hpp"
namespace Merdog
{
	namespace Object
	{
		enum OperType
		{
			_Not,
			_Add,_Sub,_Mul,_Div,
			_Inc,_Dec,_SAdd,_SSub,
			_SMul,_SDiv,_Assign,
			_Eq,_Ne,_Ge,_Gt,_Le,_Lt,
		};
		enum ObjectType
		{
			_Int, _Double, _Long, _Bool, _String, _Char,_Ref,
			_Class
		};
		class ObjectBase
		{
		public:
			ObjectBase(ObjectType ot) :obj_type(ot) {}
			virtual ~ObjectBase() {}

			virtual std::string to_string()const { return ""; };
			virtual void find_member(const std::string &str) {};
			virtual void count(ObjectBase *val, OperType type, ObjectBase *out) {};
			ObjectType obj_type;
		};
		class Int :public ObjectBase
		{
		public:
			Int(mint v) :ObjectBase(_Int),value(v) {}
			Int(Value v) :ObjectBase(_Int),value(v.to_int()) {}
			std::string to_string()const override{
				return std::to_string(value);
			}
			void count(ObjectBase *val, OperType type, ObjectBase *out) override;

			int value;
		};
		class Double :public ObjectBase
		{
		public:
			Double(mdouble v) :ObjectBase(_Double), value(v) {}
			Double(Value v) :ObjectBase(_Double), value(v.to_double()) {}
			std::string to_string()const override {
				return std::to_string(value);
			}
			void count(ObjectBase *val, OperType type, ObjectBase *out) override;

			double value;
		};
		class String :public ObjectBase
		{
		public:
			String(mstring v) :ObjectBase(_String), value(v) {}
			String(Value v) :ObjectBase(_String), value(v.to_string()) {}
			std::string to_string()const override {
				return value;
			}
			void count(ObjectBase *val, OperType type, ObjectBase *out) override;

			mstring value;
		};
		class Long :public ObjectBase
		{
		public:
			Long(mlong v) :ObjectBase(_String), value(v) {}
			Long(Value v) :ObjectBase(_Long), value(v.to_long()) {}
			std::string to_string()const override {
				return std::to_string(value);
			}
			void count(ObjectBase *val, OperType type, ObjectBase *out) override;
	
			mlong value;
		};
		class Bool :public ObjectBase
		{
		public:
			Bool(mbool v) :ObjectBase(_Bool), value(v) {}
			Bool(Value v) :ObjectBase(_Bool), value(v.to_bool()) {}
			std::string to_string()const override {
				return std::to_string(value);
			}
			void count(ObjectBase *val, OperType type, ObjectBase *out) override;

			bool value;
		};
		class Char :public ObjectBase
		{
		public:
			Char(mchar v) :ObjectBase(_Char), value(v) {}
			Char(Value v) :ObjectBase(_Char), value(v.to_char()) {}
			std::string to_string()const override {
				return std::string(1, value);
			}
			void count(ObjectBase *val, OperType type, ObjectBase *out) override;
	
			char value;
		};
	}
}