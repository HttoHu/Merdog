#include "../includes/unary_op.hpp"
namespace Mer {
	UnaryOp::UnaryOp(Token* tok, ParserNode* _node) :ParserNode(NodeType::UNARY_OP), node(_node),op_tag(tok->get_tag())
	{
		auto it = Op::UnaryOp::get_unary_op(tok->get_tag(), node->get_type());

		if (it.second == nullptr)
		{
			Type* ty = type_tab[node->get_type()];
			throw Error("invalid unary operation : " + tok->to_string() + " " + ty->get_name() + " ");
		}
		res_type = it.first;
		op_func = it.second;
	}

	void UnaryOp::execute(char* ret)
	{
		node->execute(ret);
		op_func(ret);
	}

	std::string UnaryOp::to_string() const
	{
		return "(" + TagStr[op_tag] + " " + node->to_string() + ")";
	}
}