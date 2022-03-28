#pragma once
#include "./parser_node.hpp"
#include <algorithm>
namespace Mer {
	/*
		precedence table: https://en.cppreference.com/w/cpp/language/operator_precedence
	*/

	namespace Parser {
		ParserNode* parse_expr();
		// ||
		ParserNode* parse_or();
		// &&
		ParserNode* parse_and();
		// |
		ParserNode* parse_bitwise_or();
		// ^
		ParserNode* parse_xor();
		// &
		ParserNode* parse_bitwise_and();
		// == !=
		ParserNode* parse_eq_ne();
		// >= , <= , > , <
		ParserNode* parse_cmp();
		// >> , <<
		ParserNode* parse_shift();
		// + -
		ParserNode* parse_plus_minus();
		// * / %
		ParserNode* parse_mul_div_mod();

		ParserNode* parse_unit();
	}
}