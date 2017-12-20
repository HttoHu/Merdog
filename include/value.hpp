#pragma once
#include <iostream>
#include <string>
#include "error.hpp"
typedef int mint;
typedef int64_t mlong;
typedef char mchar;
typedef std::string mstring;
typedef bool mbool;
typedef double mdouble;
namespace Merdog
{
	// the class is to store literal value.
	class Value
	{
	public:
		enum lit_type { INT, LONG, CHAR, STRING, BOOL, DOUBLE };
		Value(mint v) {
			data = new int(v);
			lt = INT;
		}
		Value(mlong v) {
			data = new mlong(v);
			lt = LONG;
		}
		Value(mchar v) {
			data = new mchar(v);
			lt = CHAR;
		}
		Value(mstring v) {
			data = new mstring(v);
			lt = STRING;
		}
		Value(mdouble v) {
			data = new mdouble(v);
			lt = DOUBLE;
		}
		Value(const Value &v)
		{
			lt = v.lt;
			switch (v.lt)
			{
			case INT:
				data = new mint(v.to_int());
				break;
			case DOUBLE:
				data = new mdouble(v.to_double());
				break;
			case CHAR:
				data = new mchar(v.to_char());
			case LONG:
				data = new mlong(v.to_long());
				break;

			case BOOL:
				data = new mbool(v.to_bool());
				break;
			case STRING:

				data = new mstring(v.to_string());
				break;
			default:
				throw Error("type convertion error.");
				break;
			}
		}
		Value &		operator=(const Value &v)
		{
			if (data == v.data)
				return *this;
			delete_value();
			lt = v.lt;
			switch (v.lt)
			{
			case INT:
				data = new mint(v.to_int());
				return *this;
			case DOUBLE:
				data = new mdouble(v.to_double());
				return *this;
			case CHAR:
				data = new mchar(v.to_char());
			case LONG:
				data = new mlong(v.to_long());
				return *this;
			case BOOL:
				data = new mbool(v.to_bool());
				return *this;
			case STRING:

				data = new mstring(v.to_string());
				return *this;
			default:
				throw Error("type convertion error.");
				break;
			}
		}
		~Value() { 
			delete_value();
		}
		lit_type	get_type()const
		{
			return lt;
		}
		bool		check_type(lit_type ot)
		{
			return ot == lt;
		}
		void		delete_value()
		{
			switch (lt)
			{
			case INT:
				delete static_cast<mint*>(data);
				return;
			case DOUBLE:
				delete static_cast<mdouble*>(data);
				return;
			case LONG:
				delete static_cast<mlong*>(data);
				return;
			case CHAR:
				delete static_cast<mchar*>(data);
				return;
			case STRING:
				delete static_cast<mstring*>(data);
				return;
			case BOOL:
				delete static_cast<mbool*>(data);
				return;
			default:
				throw std::runtime_error("cannot delete the class Value's value!");
			}
		}
		std::string to_string()const{
			switch (lt) {
			case INT:
				return std::to_string(*static_cast<mint*>(data));
			case LONG:
				return std::to_string(*static_cast<mlong*>(data));
			case CHAR:
				return std::to_string(*static_cast<mchar*>(data));
			case STRING:
				return *static_cast<mstring*>(data);
			case BOOL:
				return std::to_string(*static_cast<mbool*>(data));
			case DOUBLE:
				return std::to_string(*static_cast<mdouble*>(data));

			}
		}
		mint		to_int()const{
			switch (lt) {
			case INT:
				return *static_cast<mint*>(data);
			case LONG:
				return *static_cast<mlong*>(data);
			case CHAR:
				return *static_cast<mchar*>(data);
			case STRING:
				throw Error("string can not convert to int");
			case BOOL:
				throw Error("bool can not convert to int");
			case DOUBLE:
				return *static_cast<mbool*>(data);
			}
		}
		mlong		to_long()const
		{
			switch (lt) {
			case INT:
				return *static_cast<mint*>(data);
			case LONG:
				return *static_cast<mlong*>(data);
			case CHAR:
				return *static_cast<mchar*>(data);
			case STRING:
				throw Error("string can not convert to int");
			case BOOL:
				throw Error("bool can not convert to int");
			case DOUBLE:
				return *static_cast<mbool*>(data);
			}
		}
		mdouble		to_double()const
		{
			switch (lt) {
			case INT:
				return *static_cast<mint*>(data);
			case LONG:
				return *static_cast<mlong*>(data);
			case CHAR:
				return *static_cast<mchar*>(data);
			case STRING:
				throw Error("string can not convert to int");
			case BOOL:
				throw Error("bool can not convert to int");
			case DOUBLE:
				return *static_cast<mbool*>(data);
			}
		}
		mbool		to_bool()const
		{
			if (lt == BOOL)
				return static_cast<mbool*>(data);
			throw Error("can not convert to bool");
		}
		mchar		to_char()const
		{
			switch (lt) {
			case INT:
				return *static_cast<mint*>(data);
			case LONG:
				return *static_cast<mlong*>(data);
			case CHAR:
				return *static_cast<mchar*>(data);
			case STRING:
				throw Error("string can not convert to int");
			case BOOL:
				throw Error("bool can not convert to int");
			case DOUBLE:
				return *static_cast<mbool*>(data);
			}
		}

	private:
		void *data;
		lit_type lt;
	};
}
