#pragma once
#include <iostream>
#include "./parser_node.hpp"
#include "./lexer.hpp"
#include "./operator.hpp"

namespace Mer {
	class UnaryOp : public ParserNode {
	public:
		UnaryOp(Tag tag, ParserNode* _node);
		size_t need_space()override { return node->need_space(); }
		bool constant()const override { return node->constant(); }
		type_code_index get_type()const override { return res_type; }
		// run left node and right node first, and merge the result by op_func.
		void execute(char*)override;
		std::string to_string()const override;
	private:
		type_code_index res_type;
		ParserNode* node;
		Tag op_tag;
		Op::UnaryOp::uop_type op_func;
	};

	class CastOp : public ParserNode {
	public:
		CastOp(ParserNode* _node, type_code_index _dest_type);
		size_t need_space()override { return node->need_space(); }
		bool constant()const override { return node->constant(); }
		type_code_index get_type()const override { return res_type; }
		// run left node and right node first, and merge the result by op_func.
		void execute(char*)override;
		std::string to_string()const override;
	private:
		type_code_index res_type;
		ParserNode* node;
		Op::UnaryOp::uop_type op_func;
	};
}