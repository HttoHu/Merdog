#include "../includes/expr.hpp"
#include "../includes/operator.hpp"

namespace Mer {

	BinOp::BinOp(Token* tok, ParserNode* _left, ParserNode* _right, size_t _pos)
		: op_tag(tok->get_tag()), left(_left), right(_right), pos(_pos)
	{
		auto it = Op::get_bin_op(tok->get_tag(), left->get_type(), right->get_type());
		if (it.second == nullptr)
		{
			Type* lt = type_tab[left->get_type()];
			Type* rt = type_tab[right->get_type()];
			throw Error("invalid binary operation : " + lt->get_name() + " " + tok->to_string() + " " + rt->get_name());
		}
		res_type = it.first;
		op_func = it.second;
		left_size = left->need_space();
		right_size = right->need_space();
	}

	void BinOp::execute(char* src)
	{
		left->execute(src - left_size);
		right->execute(src - left_size - right_size);
		op_func(src - left_size, src - left_size - right_size, src - left_size - right_size);
	}
}