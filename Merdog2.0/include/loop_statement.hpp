/*
*		Inspired by
*		https://ruslanspivak.com/lsbasi-part10/
*		Ruslan's Blog
*		C++ Version.
*		Yuantao Hu 2018
*		Email :Huyuantao@outlook.com
*/
#pragma once
#include "parser.hpp"
namespace Mer
{
	class Word :public AST
	{
	public:
		enum Type
		{
			Break, Continue
		};
		Word(Type v) :type(v) {}
		Mem::Object get_value()override
		{
			throw *this;
		}
		Type type;
	};
	class While :public AST
	{
	public:
		Mem::Object get_value()override
		{
			blo->new_block();
			while (std::static_pointer_cast<Mem::Bool>(condition->get_value())->_value())
			{
				try
				{
					blo->get_value();
				}
				catch (Word c)
				{
					if (c.type == Word::Type::Break)
						break;
					else if (c.type == Word::Type::Continue)
						continue;
				}
			}
			blo->end_block();
			return nullptr;
		}
		Expr *condition;
		Block *blo;
	};
	class For :public AST
	{
	public:
		Mem::Object get_value()override
		{
			blo->new_block();
			for (init->get_value();
				std::static_pointer_cast<Mem::Bool>(condition->get_value())->_value();
				step_action->get_value())
			{
				try
				{
					blo->get_value();
				}
				catch (Word c)
				{
					if (c.type == Word::Type::Break)
						break;
					else if (c.type == Word::Type::Continue)
						continue;
				}
			}
			blo->end_block();
			return nullptr;
		}

		VarDecl * init;
		Expr *condition;
		AST *step_action;
		Block *blo;
	};
	namespace Parser
	{
		While *while_statement();
		For * for_statement();
	}
}
