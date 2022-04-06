#include "../includes/expr.hpp"
#include "../includes/binary_op.hpp"
#include "../includes/unary_op.hpp"
#include "../includes/memory.hpp"
#include "../includes/unit.hpp"
#include "../includes/symbols.hpp"
#include "../includes/environment.hpp"

#include <vector>

namespace Mer {

	namespace Parser {
		ParserNode* parse_expr()
		{
			return new Expr(Mem::default_mem.var_idx, parse_assign());
		}
		ParserNode* parse_assign()
		{
			std::set<Tag> assign_set = {
				ASSIGN,SADD,SSUB,SMUL,SDIV,SMOD,SLSH,SRSH,SAND,SOR,SXOR
			};
			auto node = parse_or();
			while (assign_set.count(token_stream.this_tag()))
			{
				auto tok = token_stream.this_token();
				token_stream.next();
				node = new AssignOp(tok,node, parse_or());
			}
			return node;
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
		ParserNode* parse_id();
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
			case PLUS:
			case NOT:
			case MINUS:
			case BNOT:
				token_stream.next();
				return new UnaryOp(result->get_tag(), parse_unit());
				// Debug
			case PRINT:
				token_stream.match(PRINT);
				return new Print(parse_expr());
			case ID:
			{
				return parse_id();
			}
			default:
				throw Error("invalid expression, unexpected token: " + token_stream.this_token()->to_string());
			}
		}
		ParserNode* parse_id() {
			std::string var_name = Id::get_value(token_stream.this_token());
			token_stream.match(ID);
			auto symbol_info = Env::symbol_table->find(var_name);
			if (symbol_info->get_word_type() == Symbol::Variable) {
				auto var_info = static_cast<Symbol::VarRecorder*>(symbol_info);
				size_t* base_ptr;
				if (var_info->is_global)
					base_ptr = &Mem::default_mem.bp;
				else
					base_ptr = &Mem::default_mem.sp;
				return new Variable(var_name, var_info->get_type(), var_info->get_pos(), base_ptr);
			}
			else if(symbol_info->get_word_type()==Symbol::Const){
				return Symbol::ConstRecorder::get_node(symbol_info)->clone();
			}
		}
	}

	void Expr::execute(char* dat)
	{
		dat = Mem::default_mem.buf + Mem::default_mem.sp + pos;
		node->execute(dat);
	}

	bool Expr::get_bool_value()
	{
		char *dat  = Mem::default_mem.buf + Mem::default_mem.sp + pos;
		node->execute(dat);
		return *(int_default*)(dat);
	}

}

