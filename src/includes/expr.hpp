#pragma once
#include "./parser_node.hpp"
#include "./lexer.hpp"
#include "./operator.hpp"
#include <algorithm>
namespace Mer {
	// A+B Problem ... 
	class BinOp :public ParserNode {
	public:
		BinOp(Token* tok, ParserNode* _left, ParserNode* _right);
		size_t need_space()override;
		bool constant()const override { return left->constant() && right->constant(); }
		type_code_index get_type()override { return res_type; }
		// run left node and right node first, and merge the result by op_func.
		void execute(char *)override;
		std::string to_string()const override;
	private:
		type_code_index res_type;
		ParserNode* left;
		ParserNode* right;
		Tag op_tag;
		Op::op_type op_func;

		size_t left_size;
	};

	class LogicalBinOp :public ParserNode {
	public:
		LogicalBinOp(Token *tok, ParserNode* _left, ParserNode* _right);
		size_t need_space()override;
		bool constant()const override { return left->constant() && right->constant(); }
		type_code_index get_type()override { return (type_code_index) BasicTypeTag::INT; }
		// run left node and right node first, and merge the result by op_func.
		void execute(char*)override;
		std::string to_string()const override;
	private:
		ParserNode* left;
		ParserNode* right;
		int is_and_op;
		Op::op_type op_func;

		size_t left_size;
	};
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