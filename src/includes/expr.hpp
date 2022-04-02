#pragma once
#include "./parser_node.hpp"
#include <algorithm>
namespace Mer {
	/*
		precedence table: https://en.cppreference.com/w/cpp/language/operator_precedence
	*/
	class Expr :public ParserNode {
	public:
		Expr(size_t _pos,ParserNode* _node) :ParserNode(NodeType::EXPR), pos(_pos),node(_node) {}
		type_code_index get_type()const override {
			return node->get_type();
		}
		size_t get_pos()override { return node->get_pos(); }
		void execute(char*)override;
		char* get_runtime_pos()override { return node->get_runtime_pos(); }
		bool constant()const override { return node->actual_space(); }
		size_t need_space()override { return node->need_space(); }
		size_t actual_space()override { return node->actual_space(); }
		std::string to_string()const override { return node->to_string(); }

	private:
		ParserNode* node;
		size_t pos;
	};
	namespace Parser {
		ParserNode* parse_expr();
		// =,+=,-=,/=,*=,%=, >>= ,<<= ,&=,^=,|=
		ParserNode* parse_assign();
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