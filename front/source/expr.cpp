#include "../include/expr.hpp"
#include "../include/type.hpp"
#include "../include/lexer.hpp"
namespace mer
{
	extern std::string get_tmp_var_name( bool is_c=false);
	extern std::vector<std::string> ir_instructions;
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
	BinOp::BinOp(Node l, Token* _op, Node r) :ParserNode(BINOP), left(l), op(_op), right(r)
	{

		set_type(left->get_type());
		if (get_type() == nullptr)
			throw Error("我日了狗了");
	}
	void BinOp::emit_gen()
	{
		if (!left->get_type()->convertible(right->get_type()))
			throw Error("type not matched from " + left->get_type()->name() + " to " + right->get_type()->name());
		left->emit_gen();
		right->emit_gen();
		std::string right_name = right->to_string();
		std::string left_name = left->to_string();
		// if left_node's type isn't match with expr type; convert it 
		if (!get_type()->same(left->get_type()))
		{
			left_name=type_convert(left->get_type(), get_type(), left);
		}
		// if right node's type isn't match with expr type; convert it 
		if (!get_type()->same(right->get_type()))
		{
			right_name = type_convert(right->get_type(), get_type(), right);
		}
		std::string sign = tag_to_sign(op->get_tag());
		if (_is_assign(op->get_tag()))
		{
			ir_instructions.push_back(left_name + sign + right_name);
			return;
		}
		var_name = get_tmp_var_name();
		// type name=left+right
		ir_instructions.push_back(get_type()->name() + " " + var_name + "=" + left_name + sign + right_name );
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
			case TTRUE:
			case TFALSE:
				return BoolVNode(new BoolV);
			case INTEGER:
				return IntVNode(new IntV); 
			case REAL:
				return RealVNode(new RealV);
			case CHAR:
				return CharVNode(new CharV);
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
