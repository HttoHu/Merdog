#include "../include/expr.hpp"
#include "../include/type.hpp"
#include "../include/lexer.hpp"
namespace mer
{
	std::string get_tmp_var_name(bool clear = false);
	IntV::IntV(int v) :value(v), ParserNode(ParserNode::CINTV) 
	{
		set_type(type_map["int"] );
	}
	IntV::IntV():ParserNode(ParserNode::CINTV)
	{
		value = Integer::get_value(token_stream.this_token());
		token_stream.next();
		set_type(type_map["int"]);

	}
	std::string IntV::to_string()
	{
		return "i" + std::to_string(value);
	}
	std::string BinOp::get_gen()
	{
		std::string ret;
		ret+=left->get_gen();
		ret+=right->get_gen();
		var_name = get_tmp_var_name();
		// type name=left+right
		ret += get_type()->name()+" "+ var_name+"=";
		// get sign
		std::string sign = tag_to_sign(op->get_tag());
		ret += left->to_string() + sign + right->to_string() + '\n';
		return ret;
	}
	namespace analyse_expr
	{
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
				auto ret = expr();
				token_stream.match(RPAREN);
				return ret;
			}
			default:
				throw Error("<syntax error!>");
			}
		}
	}
}
