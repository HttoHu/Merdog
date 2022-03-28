#pragma once
#include <map>
#include "./type.hpp"
#include "./lexer.hpp"

namespace Mer {
	namespace Op {
		namespace BinOp {
			using bop_type = void (*)(char*, char*, char*);

			template<typename RT, typename T1, typename T2>
			void add(char* l, char* r, char* ret)
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
			template<typename RT, typename T1, typename T2>
			void logical_and(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) && (*(T2*)r);
			}

			template<typename RT, typename T1, typename T2>
			void logical_or(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) || (*(T2*)r);
			}

			template<typename RT, typename T1, typename T2>
			void bitwise_and(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) & (*(T2*)r);
			}
			template<typename RT, typename T1, typename T2>
			void bitwise_or(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) | (*(T2*)r);
			}
			template<typename RT, typename T1, typename T2>
			void bitwise_xor(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) ^ (*(T2*)r);
			}

			template<typename RT, typename T1, typename T2>
			void left_shift(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) << (*(T2*)r);
			}
			template<typename RT, typename T1, typename T2>
			void right_shift(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) >> (*(T2*)r);
			}

			template<typename RT, typename T1, typename T2>
			void eq(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) == (*(T2*)r);
			}
			template<typename RT, typename T1, typename T2>
			void ne(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) != (*(T2*)r);
			}
			template<typename RT, typename T1, typename T2>
			void ge(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) >= (*(T2*)r);
			}
			template<typename RT, typename T1, typename T2>
			void le(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) <= (*(T2*)r);
			}
			template<typename RT, typename T1, typename T2>
			void gt(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) > (*(T2*)r);
			}
			template<typename RT, typename T1, typename T2>
			void lt(char* l, char* r, char* ret)
			{
				*(RT*)ret = (*(T1*)l) < (*(T2*)r);
			}
			// return the function and the function ret type
			std::pair<type_code_index, bop_type> get_bin_op(Tag op, type_code_index lt, type_code_index rt);
		}
		namespace UnaryOp {
			using uop_type = void (*) (char*);
			template<typename RT, typename T>
			void not_op(char* v) {
				*(RT*)v = !*(T*)v;
			}
			template<typename RT, typename T>
			void get_neg(char* v) {
				*(RT*)v = -*(T*)v;
			}
			template<typename RT, typename T>
			void bitwise_not(char* v) {
				*(RT*)v = ~*(T*)v;
			}
			std::pair<type_code_index, uop_type> get_unary_op(Tag op, type_code_index ty);
		}
	}
}