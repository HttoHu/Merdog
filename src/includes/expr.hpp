#pragma once
#include "./parser_node.hpp"
#include "./lexer.hpp"
#include "./operator.hpp"
#include <algorithm>
namespace Mer {
	// A+B Problem ... 
	class BinOp :public ParserNode {
	public:
		BinOp(Token* tok, ParserNode* _left, ParserNode* _right,size_t _pos);
		size_t need_space()override {
			return std::max(left_size+ right_size,type_tab[res_type]->type_length());
		}

		bool constant()const override { return left->constant() && right->constant(); }
		type_code_index get_type()override { return res_type; }
		// run left node and right node first, and merge the result by op_func.
		void execute(char *)override;
	private:
		size_t pos;
		type_code_index res_type;
		ParserNode* left;
		ParserNode* right;
		Tag op_tag;
		Op::op_type op_func;

		size_t left_size;
		size_t right_size;
	};


	namespace Parser {
		ParserNode* parse_unit();
	}
}