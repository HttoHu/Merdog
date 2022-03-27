#include "../includes/expr.hpp"
#include "../includes/operator.hpp"
#include "../includes/unit.hpp"
#include <vector>

namespace Mer {

	BinOp::BinOp(Token* tok, ParserNode* _left, ParserNode* _right)
		:ParserNode(NodeType::BIN_OP), op_tag(tok->get_tag()), left(_left), right(_right)
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

		left_size = left->node_size();
	}

	size_t BinOp::need_space()
	{
		std::vector <size_t> vec = { left->node_size() + right->node_size(),node_size(),left->need_space(),left_size + right->need_space() };
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

	namespace Parser {
		ParserNode* parse_expr()
		{
			return parse_plus_minus();
		}
		ParserNode* parse_plus_minus()
		{
			auto result = parse_mul_div_mod();
			while (token_stream.this_tag() == PLUS || token_stream.this_tag() == MINUS)
			{
				auto tok = token_stream.this_token();
				token_stream.next();
				result = new BinOp(tok, result, parse_mul_div_mod());
			}
			return result;
		}
		ParserNode* parse_mul_div_mod()
		{
			auto result = parse_unit();
			while (token_stream.this_tag() == MUL || token_stream.this_tag() == DIV || token_stream.this_tag() == MOD)
			{
				auto tok = token_stream.this_token();
				token_stream.next();
				result = new BinOp(tok, result, parse_unit());
			}
			return result;
		}
		ParserNode* parse_unit()
		{
			auto result = token_stream.this_token();
			ParserNode* ret = nullptr;
			switch (result->get_tag())
			{
			case CHAR_LIT:
			case REAL:
			case INTEGER:
			case TTRUE:
			case TFALSE:
				token_stream.next();
				return new LConV(result);
			case LPAREN:
				token_stream.match(LPAREN);
				ret = parse_expr();
				token_stream.match(RPAREN);
				return ret;
			}
		}

	}
}

