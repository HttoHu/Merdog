#include "../includes/unary_op.hpp"
namespace Mer {
	UnaryOp::UnaryOp(Tag tag, ParserNode* _node) :ParserNode(NodeType::UNARY_OP), node(_node),op_tag(tag)
	{
		auto res = Op::UnaryOp::get_unary_op(tag, node->get_type());

		if (res.second == nullptr)
		{
			Type* ty = type_tab[node->get_type()];
			throw Error("invalid unary operation : " + TagStr[tag] + " " + ty->get_name() + " ");
		}
		res_type = res.first;
		op_func = res.second;
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
	CastOp::CastOp(ParserNode* _node, type_code_index _dest_type):ParserNode(NodeType::CAST),node(_node),res_type(_dest_type)
	{
		op_func = Op::UnaryOp::get_cast_op(node->get_type(), res_type);
		if (op_func == nullptr)	
		{
			Type* ty = type_tab[node->get_type()];
			Type* ty2 = type_tab[node->get_type()];
			throw Error("invalid cast operation : cast from " + ty->get_name() + " to "+ty2->get_name() );
		}

	}
	void CastOp::execute(char* ret)
	{
		node->execute(ret);
		op_func(ret);
	}
	std::string CastOp::to_string() const
	{
		return "(cast " + node->to_string() + " to " + type_tab[res_type]->get_name() + ")";
	}
}