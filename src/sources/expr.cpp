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


	LogicalBinOp::LogicalBinOp(Token* tok, ParserNode* _left, ParserNode* _right):ParserNode(NodeType::LOGICAL_BIN),left(_left),right(_right)
	{
		is_and_op = tok->get_tag() == AND;
		auto res = Op::get_bin_op(tok->get_tag(), left->get_type(), right->get_type());

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

	namespace Parser {
		ParserNode* parse_expr()
		{
			return parse_or();
		}
		ParserNode* parse_or()
		{
			auto result = parse_and();
			while (token_stream.this_tag() == OR)
			{
				auto tok = token_stream.this_token();
				token_stream.next();
				result = new LogicalBinOp(tok, result, parse_and());
			}
			return result;
		}
		ParserNode* parse_and()
		{
			auto result = parse_bitwise_or();
			while (token_stream.this_tag() == AND)
			{
				auto tok = token_stream.this_token();
				token_stream.next();
				result = new LogicalBinOp(tok, result, parse_bitwise_or());
			}
			return result;
		}
		ParserNode* parse_bitwise_or()
		{
			auto result = parse_xor();
			while (token_stream.this_tag() == BOR)
			{
				auto tok = token_stream.this_token();
				token_stream.next();
				result = new BinOp(tok, result, parse_xor());
			}
			return result;
		}
		ParserNode* parse_xor()
		{
			auto result = parse_bitwise_and();
			while (token_stream.this_tag() == BXOR)
			{
				auto tok = token_stream.this_token();
				token_stream.next();
				result = new BinOp(tok, result, parse_bitwise_and());
			}
			return result;
		}
		ParserNode* parse_bitwise_and()
		{
			auto result = parse_eq_ne();
			while (token_stream.this_tag() == BAND)
			{
				auto tok = token_stream.this_token();
				token_stream.next();
				result = new BinOp(tok, result, parse_eq_ne());
			}
			return result;
		}
		ParserNode* parse_eq_ne()
		{
			auto result = parse_cmp();
			while (token_stream.this_tag() == NE || token_stream.this_tag() == EQ)
			{
				auto tok = token_stream.this_token();
				token_stream.next();
				result = new BinOp(tok, result, parse_cmp());
			}
			return result;
		}
		ParserNode* parse_cmp()
		{
			auto result = parse_shift();
			while (token_stream.this_tag() == LT || token_stream.this_tag() == GT || token_stream.this_tag() == LE || token_stream.this_tag() == GE)
			{
				auto tok = token_stream.this_token();
				token_stream.next();
				result = new BinOp(tok, result, parse_shift());
			}
			return result;
		}
		ParserNode* parse_shift()
		{
			auto result = parse_plus_minus();
			while (token_stream.this_tag() == LSH || token_stream.this_tag() == RSH)
			{
				auto tok = token_stream.this_token();
				token_stream.next();
				result = new BinOp(tok, result, parse_plus_minus());
			}
			return result;
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
			default:
				throw Error("invalid expression, unexpected token: "+token_stream.this_token()->to_string());
			}
		}

	}

}

