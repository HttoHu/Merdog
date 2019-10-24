#include "../include/parser.hpp"
#include "../include/lexer.hpp"
#include "../include/type.hpp"
#include "../include/symbol_table.hpp"
namespace mer
{

	namespace analyse_expr {
		Node create_expr();
	}

	bool _is_value_node(ParserNode::NodeTag t)
	{
		switch (t)
		{
		case mer::ParserNode::VARIABLE:
		case mer::ParserNode::CINTV:
		case mer::ParserNode::REAL_VALUE:
		case mer::ParserNode::BOOL_VALUE:
		case mer::ParserNode::CHAR_VALUE:
			return true;
		default:
			return false;
		}
	}
	NamePart::NamePart()
	{
		name = Id::get_value(token_stream.this_token());
		token_stream.next();
	}
	VarDeclUnit::VarDeclUnit() :name_part()
	{
		if (token_stream.this_tag() == COMMA)
			return;
		else if (token_stream.this_tag() == ASSIGN)
		{
			token_stream.match(ASSIGN);
			expr = analyse_expr::create_expr();
		}
		else
			throw Error("illegal init expr!");
	}
	LocalVarDecl::LocalVarDecl(std::vector<VarDeclUnitPtr>& vec, Type* t):ParserNode(LOCAL_VARDECl), units(vec)
	{
		for (const auto& a : vec)
		{
			_symbol_table.push(new IdRecorder(a->get_name_part().get_name(), t));
		}
		set_type(t);
	}
	void LocalVarDecl::emit_gen()
	{
		std::string type_name = get_type()->name();
		std::string ret;
		for (auto& a : units)
		{
			std::string var_name = a->get_name_part().get_name();
			a->expr->emit_gen();
			ret += type_name + " %" + var_name;
			if (a->expr->get_tag() != NONOP)
			{
				// if the expr is bin , try to assign a var type to it. we don't assign type to value_node, cause the different mechanism to 
				// binop and value_node
				if(a->expr->get_tag()==BINOP)
					a->expr->set_type(get_type());
				std::string tmp = a->expr->to_string();
				if (!a->expr->get_type()->same(get_type())&&_is_value_node(a->expr->get_tag()))
				{
					tmp = type_convert(a->expr->get_type(), get_type(), a->expr);
				}
				ret += "=" + tmp;
			}
			ir_instructions.push_back(ret);
		}
	}

	namespace parse_statement
	{
		Node statement()
		{
			switch (token_stream.this_tag())
			{
			case INTEGER_DECL:
			case STRING_DECL:
			case BOOL_DECL:
			case CHAR_DECL:
			case REAL_DECL:
				return var_decl();
			default:
			{
				auto ret= analyse_expr::create_expr();
				token_stream.match(SEMI);
				return ret;
			}
			}
		}
		Node var_decl()
		{
			Type* type = mer::process_type();
			std::vector<VarDeclUnitPtr> units;
			units.push_back(VarDeclUnitPtr(new VarDeclUnit()));
			while (token_stream.this_tag() != SEMI)
			{
				token_stream.match(COMMA);
				units.push_back(VarDeclUnitPtr(new VarDeclUnit()));
			}
			token_stream.match(SEMI);
			return LocalVarDeclPtr(new LocalVarDecl(units, type));
		}
	}
}


