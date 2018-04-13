#pragma once
#include "parser_node.hpp"
#include "lexer.hpp"
namespace Mer
{
	class Num:public ParserNode
	{
	public:
		Num(Token *t) :tok(t) {}
		Mem::Object execute()override
		{
			switch (tok->get_tag())
			{
			case TRUE:
				return std::make_shared<Mem::Bool>(true);
			case FALSE:
				return std::make_shared<Mem::Bool>(false);
			case INTEGER:
				return std::make_shared<Mem::Int>(Integer::get_value(tok));
			case REAL:
				return std::make_shared <Mem::Double >(Real::get_value(tok));
			case STRING:
				return std::make_shared<Mem::String>(String::get_value(tok));
			default:
				throw Error("syntax error");
			}
		}
		std::string to_string()
		{
			return tok->to_string();
		}
	private:
		Token *tok;
	};
	class Variable :public ParserNode
	{
	public:
		Variable(Token *tok);
		Mem::Object execute()override;
	private:
		size_t pos;
	};
}