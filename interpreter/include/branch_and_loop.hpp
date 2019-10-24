#pragma once
#include "parser.hpp"
#include <memory>
#include <map>
namespace Mer
{
	using PosPtr = std::shared_ptr<size_t>;
	extern std::vector<ParserNode*>* current_ins_table;
	extern size_t current_function_rety;
	extern PosPtr this_block_size;
	extern Mem::Object function_ret;
	class Expr;
	class Return : public ParserNode
	{
	public:
		Return(size_t* _pc, Expr* _expr);
		Mem::Object execute()override;
	private:
		PosPtr des;
		size_t* pc;
		Expr* expr;
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
		std::string to_string()override;
		size_t* pc;
		PosPtr default_pos;
	private:
		ParserNode* expr;
	};
	class IfTrueToAOrB :public ParserNode
	{
	public:
		IfTrueToAOrB(size_t* _pc, PosPtr _true_tag, PosPtr _false_tag, ParserNode* _expr) : pc(_pc), true_tag(_true_tag), false_tag(_false_tag), expr(_expr) {}
		Mem::Object execute()override;
		std::string to_string()override;
	private:
		size_t* pc;
		PosPtr true_tag;
		PosPtr false_tag;
		ParserNode* expr;
	};
	// push a true value and a size_t to set else pos
	class IfWithJmpTable :public ParserNode
	{
	public:
		IfWithJmpTable(size_t* _pc) :pc(_pc) {}
		Mem::Object execute()override;
		std::string to_string()override;
		std::vector<std::pair<ParserNode*, PosPtr>> jmp_table;
		PosPtr end_pos;
	private:
		size_t* pc;

	};
	namespace Parser
	{
//pdel
		void build_block();
		// you need new block by hand. In an effort to push argument into the block before the BEGIN.
		void build_function_block();
		void do_while();
		void build_while();
		void build_if();
	}
}