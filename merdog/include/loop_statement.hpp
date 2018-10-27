#pragma once
#include "parser.hpp"
namespace Mer
{
	class Word :public ParserNode
	{
	public:
		enum WType
		{
			Break, Continue
		};
		Word(WType v) :type(v) {}
		Mem::Object execute()override
		{
			throw *this;
		}
		WType type;
	};
	class While :public ParserNode
	{
	public:
		Mem::Object execute()override
		{
			blo->new_block();
			while (std::static_pointer_cast<Mem::Bool>(condition->execute())->_value())
			{
				try
				{
					blo->execute();
				}
				catch (Word c)
				{
					if (c.type == Word::WType::Break)
						break;
					else if (c.type == Word::WType::Continue)
						continue;
				}
			}
			blo->end_block();
			return nullptr;
		}
		Expr *condition;
		Block *blo;
	};
	class For :public ParserNode
	{
	public:
		Mem::Object execute()override
		{
			blo->new_block();
			for (init->execute();
				std::static_pointer_cast<Mem::Bool>(condition->execute())->_value();
				step_action->execute())
			{
				try
				{
					blo->execute();
				}
				catch (Word c)
				{
					if (c.type == Word::WType::Break)
						break;
					else if (c.type == Word::WType::Continue)
						continue;
				}
			}
			blo->end_block();
			return nullptr;
		}

		ParserNode * init;
		Expr *condition;
		ParserNode *step_action;
		Block *blo;
	};
	namespace Parser
	{
		While *while_statement();
		For * for_statement();
	}
}