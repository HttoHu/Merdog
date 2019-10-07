/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/expr.hpp"
#include "../include/if.hpp"
#include "../include/memory.hpp"
namespace Mer
{
	namespace Parser
	{
		ParserNode * if_statement()
		{
			If *ret = new If();
			token_stream.match(IF);
			token_stream.match(LPAREN);
			auto expr = new Expr();
			token_stream.match(RPAREN);
			mem.new_block();
			auto blo = Parser::block();
			ret->if_block.push_back({ expr,blo });
			while (token_stream.this_tag() == ELSE_IF)
			{
				token_stream.match(ELSE_IF);
				token_stream.match(LPAREN);
				auto expr = new Expr();
				token_stream.match(RPAREN);
				auto blo = Parser::block();
				ret->if_block.push_back({ expr,blo });
			}
			if (token_stream.this_tag() == ELSE)
			{
				token_stream.match(ELSE);
				ret->else_block = Parser::block();
			}
			return ret;
		}
	}
	Mem::Object If::execute()
	{
		mem.new_block();
		for (auto &a : if_block)
		{
			if (std::static_pointer_cast<Mem::Bool>(a.first->execute())->_value())
			{
				a.second->execute();
				mem.end_block();
				return nullptr;
			}
		}
		if (else_block != nullptr)
			else_block->execute();
		mem.end_block();
		return nullptr;
	}
	Mem::Object IfTrueToAOrB::execute()
	{
		if (std::static_pointer_cast<Mem::Bool>(expr->execute())->_value())
		{ 
			*pc = *true_tag-1;
		}
		else
			*pc = *false_tag-1;
		return nullptr;
	}
	std::string IfTrueToAOrB::to_string()
	{
		return "if " + expr->to_string() + " is true goto " + std::to_string(*true_tag) + " or " + std::to_string(*false_tag);
	}
	Mem::Object IfWithJmpTable::execute()
	{
		for (auto &a : jmp_table)
		{
			if (std::static_pointer_cast<Mem::Bool>(a.first->execute())->_value())
			{
				*pc = *a.second-1;
				return nullptr;
			}
		}
		return nullptr;
	}
	std::string IfWithJmpTable::to_string()
	{
		std::string ret = "if-else";
		for (auto &a : jmp_table)
		{
			ret += "(";
			ret += a.first->to_string();
			ret += " : ";
			ret += std::to_string(*a.second);
			ret += ")-";
		}
		return ret;
	}
}