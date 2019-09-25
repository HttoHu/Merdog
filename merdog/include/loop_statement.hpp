#pragma once
#include "parser.hpp"
namespace Mer
{
	class Expr;
	class Word :public ParserNode
	{
	public:
		enum Type
		{
			Break, Continue
		};
		Word(Type v) :type(v) {}
		Mem::Object execute()override
		{
			throw *this;
		}
		Type type;
	};
	class While :public ParserNode
	{
	public:
		Mem::Object execute()override;
		Expr *condition;
		Block *blo;
	};
	class For :public ParserNode
	{
	public:
		Mem::Object execute()override;

		ParserNode * init;
		Expr *condition;
		ParserNode *step_action;
		Block *blo;
	};
	class DoWhile :public ParserNode
	{
	public:
		Mem::Object execute()override;
		Expr* condition;
		Block* blo;
	};
	namespace Parser
	{
		ParserNode *while_statement();
		ParserNode * for_statement();
	}
}