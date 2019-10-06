#pragma once
#include "parser.hpp"
namespace Mer
{
	using PosPtr = std::shared_ptr<size_t>;
	class If :public ParserNode
	{
	public:
		Mem::Object execute()override;
		std::vector<std::pair<Expr*, Block*>> if_block;
		Block* else_block;
	};
	namespace Parser
	{
		ParserNode *if_statement();
	}
	class IfTrueToAOrB :public ParserNode
	{
	public:
		IfTrueToAOrB(size_t* _pc, PosPtr _true_tag, PosPtr _false_tag, Expr* _expr) : pc(_pc), true_tag(_true_tag), false_tag(_false_tag), expr(_expr) {}
		Mem::Object execute()override;
		std::string to_string()override;
	private:
		size_t *pc;
		PosPtr true_tag;
		PosPtr false_tag;
		Expr* expr;
	};
	// push a true value and a size_t to set else pos
	class IfWithJmpTable :public ParserNode
	{
	public:
		IfWithJmpTable(size_t *_pc, const std::vector<std::pair<Expr*, size_t>>& jt) :pc(_pc), jmp_table(jt) {}
		Mem::Object execute()override;
	private:
		size_t *pc;
		std::vector<std::pair<Expr*, size_t>> jmp_table;
	};
}