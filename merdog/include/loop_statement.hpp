#pragma once
#include "parser.hpp"
namespace Mer
{
	using PosPtr = std::shared_ptr<size_t>;
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
	class Continue :public ParserNode
	{
	public:
		Continue(size_t *_index, PosPtr _target) :index(_index),target(_target) {}
		Mem::Object execute();
		std::string to_string()override;
	private:
		size_t *index;
		PosPtr target;
	};
	class Goto :public ParserNode
	{
	public:
		Goto(size_t *_index, PosPtr _target) :index(_index),target(_target) {}
		Mem::Object execute();
		std::string to_string()override;
	private:
		size_t *index;
		PosPtr target;
	};
	template<typename Key>
	class CaseSet: public ParserNode
	{
	public:
		CaseSet(size_t *_pc,Expr* _expr) :pc(_pc),expr(_expr) {}
		Mem::Object execute()override
		{
			auto result = jmp_table.find(expr->execute());
			if (result == jmp_table.end())
				*pc = *default_pos;
			else
				*pc = *result.second;
		}
		std::map<Key, PosPtr> jmp_table;
		size_t *pc;
		PosPtr default_pos;
	private:
		Expr* expr;
	};
	namespace Parser
	{
//pdel
		void do_while();
		void demo();
		void build_while();
		void build_if();
		ParserNode *while_statement();
		ParserNode * for_statement();
	}
}