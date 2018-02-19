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
			return nullptr;
		}
		Expr *condition;
		Block *blo;
	};
	class For :public AST
	{
	public:
	private:
	};
	namespace Parser
	{
		While *while_statement();
	}
}
