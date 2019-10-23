#include "../include/expr.hpp"
#include "../include/type.hpp"
#include "../include/lexer.hpp"
namespace mer
{
	extern std::string get_tmp_var_name( bool is_c=false);
	inline bool _is_assign(Tag t)
	{
		switch (t)
		{
		case ASSIGN:
		case SADD:
		case SSUB:
		case SMUL:
		case SDIV:
			return true;
		default:
			return false;
		}
	}
	std::string BinOp::get_gen()
	{
		std::string ret;
		ret+=left->get_gen();
		ret+=right->get_gen();
		std::string sign = tag_to_sign(op->get_tag());
		// if op is assign-series operation
		if (_is_assign(op->get_tag()))
		{
			ret += left->to_string() + sign + right->to_string() + '\n';
			return ret;
		}
		var_name = get_tmp_var_name();
		// type name=left+right
		ret += get_type()->name()+" "+ var_name+"=";
		// get sign

		ret += left->to_string() + sign + right->to_string() + '\n';
		return ret;
	}




	namespace analyse_expr
	{
		Node create_expr()
		{
			return assign();
		}
		Node assign()
		{
			Node result = expr();
			while (_is_assign(token_stream.this_tag()))
			{
				Token* tok = token_stream.this_token();
				token_stream.next();
				result = std::make_shared<BinOp>(result, tok, expr());
			}
			return result;
		}
		Node expr()
		{
			Node result=term();
			while (token_stream.this_tag()==PLUS||token_stream.this_tag()==MINUS)
			{
				Token* tok=token_stream.this_token();
				token_stream.next();
				result=std::make_shared<BinOp>(result,tok,term());
			}
			return result;
		}
		Node term()
		{
			Node result=factor();
			while (token_stream.this_tag()==MUL||token_stream.this_tag()==DIV)
			{
				Token* tok=token_stream.this_token();
				token_stream.next();
				result=std::make_shared<BinOp>(result,tok,factor());
			}
			return result;
		}
		Node factor()
		{
			switch (token_stream.this_tag())
			{
			case INTEGER:
				return IntVNode(new IntV); 
			case LPAREN:
			{
				token_stream.match(LPAREN);
				auto ret = create_expr();
				token_stream.match(RPAREN);
				return ret;
			}
			case ID:
			{
				std::string id_name = Id::get_value(token_stream.this_token());
				token_stream.next();
				return std::make_shared<Variable>(id_name);
			}
			default:
				throw Error("< "+token_stream.this_token()->to_string()+" syntax error!>");
			}
		}
	}
}
