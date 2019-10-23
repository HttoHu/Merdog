#include "../include/parser.hpp"
#include "../include/lexer.hpp"
#include "../include/type.hpp"
#include "../include/symbol_table.hpp"
namespace mer
{
	namespace analyse_expr {
		Node create_expr();
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
	std::string LocalVarDecl::get_gen()
	{
		std::string type_name = get_type()->name();
		std::string ret;
		for (auto& a : units)
		{
			std::string var_name= a->get_name_part().get_name();
			ret += a->expr->get_gen();
			ret += type_name + " %" + var_name;
			if (a->expr->get_tag() != NONOP)
			{
				ret += "=" + a->expr->to_string();
			}
			ret += "\n";
		}
		return ret;
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


