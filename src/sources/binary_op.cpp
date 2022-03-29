#include "../includes/binary_op.hpp"
#include "../includes/unary_op.hpp"

namespace Mer {

	BinOp::BinOp(Token* tok, ParserNode* _left, ParserNode* _right)
		:ParserNode(NodeType::BIN_OP), op_tag(tok->get_tag()), left(_left), right(_right)
	{
		auto [expected_ltype, expected_rtype] = Op::BinOp::op_type_converter(left->get_type(), right->get_type(), tok->get_tag());
		
		if (expected_ltype != left->get_type())
			left = new CastOp(left, expected_ltype);
		if (expected_ltype != right->get_type())
			right = new CastOp(right, expected_ltype);

		auto it = Op::BinOp::get_bin_op(tok->get_tag(), left->get_type(), right->get_type());
		if (it.second == nullptr)
		{
			Type* lt = type_tab[left->get_type()];
			Type* rt = type_tab[right->get_type()];
			throw Error("invalid binary operation : " + lt->get_name() + " " + tok->to_string() + " " + rt->get_name());
		}
		res_type = it.first;

		op_func = it.second;

		left_size = left->node_size();
	}

	size_t BinOp::need_space()
	{
		std::vector <size_t> vec = { node_size(),left->need_space(),left_size + right->need_space() };
		sort(vec.begin(), vec.end());
		return vec.back();
	}

	void BinOp::execute(char* src)
	{
		left->execute(src);
		right->execute(src + left_size);
		op_func(src, src + left_size, src);
	}

	std::string BinOp::to_string() const
	{
		return "BinOp(" + left->to_string() + " " + TagStr[op_tag] + " " + right->to_string() + ")";
	}


	LogicalBinOp::LogicalBinOp(Token* tok, ParserNode* _left, ParserNode* _right) :ParserNode(NodeType::LOGICAL_BIN), left(_left), right(_right)
	{
		is_and_op = tok->get_tag() == AND;
		auto res = Op::BinOp::get_bin_op(tok->get_tag(), left->get_type(), right->get_type());

		if (res.second == nullptr)
		{
			Type* lt = type_tab[left->get_type()];
			Type* rt = type_tab[right->get_type()];
			throw Error("invalid logical binary operation : " + lt->get_name() + " " + tok->to_string() + " " + rt->get_name());
		}
		op_func = res.second;
		left_size = left->node_size();
	}
	size_t LogicalBinOp::need_space()
	{
		std::vector <size_t> vec = { node_size(),left->need_space(),left_size + right->need_space() };
		sort(vec.begin(), vec.end());
		return vec.back();
	}
	void LogicalBinOp::execute(char* src)
	{
		left->execute(src);
		if (is_and_op ^ *(int_default*)src)
			return (void)(*(int_default*)src = !is_and_op);
		right->execute(src);
		*(int_default*)src = (bool)*(int_default*)src;
	}

	std::string LogicalBinOp::to_string() const
	{
		return "(LOGBIN " + std::string(1, "&|"[is_and_op]) + left->to_string() + " " + right->to_string() + ")";
	}

}