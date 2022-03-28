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
		void execute(char*)override;
		std::string to_string()const override;
	private:
		type_code_index res_type;
		ParserNode* left;
		ParserNode* right;
		Tag op_tag;
		Op::BinOp::bop_type op_func;

		size_t left_size;
	};

	class LogicalBinOp :public ParserNode {
	public:
		LogicalBinOp(Token* tok, ParserNode* _left, ParserNode* _right);
		size_t need_space()override;
		bool constant()const override { return left->constant() && right->constant(); }
		type_code_index get_type()override { return (type_code_index)BasicTypeTag::INT; }
		// run left node and right node first, and merge the result by op_func.
		void execute(char*)override;
		std::string to_string()const override;
	private:
		ParserNode* left;
		ParserNode* right;
		int is_and_op;
		Op::BinOp::bop_type op_func;

		size_t left_size;
	};
}