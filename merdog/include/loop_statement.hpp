#pragma once
#include "parser.hpp"
#include <memory>
#include <map>
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
	class IntCaseSet: public ParserNode
	{
	public:
		IntCaseSet(size_t *_pc,ParserNode* _expr) :pc(_pc),expr(_expr) {}
		std::map<int, PosPtr> jmp_table;
		Mem::Object execute()override;
		std::string to_string()override;
		size_t *pc;
		PosPtr default_pos;
	private:
		ParserNode* expr;
	};
	class StrCaseSet : public ParserNode
	{
	public:
		StrCaseSet(size_t* _pc, ParserNode* _expr) :pc(_pc), expr(_expr) {}
		std::map<std::string, PosPtr> jmp_table;
		Mem::Object execute()override;
		size_t* pc;
		PosPtr default_pos;
	private:
		ParserNode* expr;
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