#pragma once
#include <map>
#include "./type.hpp"
#include "./lexer.hpp"

namespace Mer {
	namespace Op {
		using op_type = void (*)(char*, char*,char*);

		template<typename RT,typename T1,typename T2>
		void add(char* l, char* r,char*ret)
		{
			*(RT*)ret = *(T1*)l + *(T2*)r;
		}
		template<typename RT, typename T1, typename T2>
		void sub(char* l, char* r, char* ret)
		{
			*(RT*)ret = *(T1*)l - *(T2*)r;
		}
		template<typename RT, typename T1, typename T2>
		void div(char* l, char* r, char* ret)
		{
			*(RT*)ret = *(T1*)l / *(T2*)r;
		}
		template<typename RT, typename T1, typename T2>
		void mul(char* l, char* r, char* ret)
		{
			*(RT*)ret = *(T1*)l * *(T2*)r;
		}
		template<typename RT, typename T1, typename T2>
		void mod(char* l, char* r, char* ret)
		{
			*(RT*)ret = (*(T1*)l) % (*(T2*)r);
		}
		// return the function and the function ret type
		std::pair<type_code_index, op_type> get_bin_op(Tag op, type_code_index lt, type_code_index rt);
	}
}