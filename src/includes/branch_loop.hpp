#pragma once

#include <string>
#include <vector>
#include "./parser_node.hpp"
#include "./defs.hpp"
namespace Mer
{
	// goto ,break and some other statements may need a loop end position. so when parsing loop over,
	// you need to change their value, wo push the unchanged position to a table and update it when the loop parsing over.
	namespace BranchAndLoop
	{
		extern std::vector<std::vector<size_t*>> _change_pos_tab;

		class Goto : public ParserNode
		{
		public:
			Goto(size_t* src, PosPtr _dest) : ParserNode(NodeType::GOTO), pc(src), dest(_dest) {}
			void execute(char*) override;
			std::string to_string()const override;

			PosPtr dest;
		private:
			size_t* pc;
		};
		class Continue :public ParserNode {
		public:
			Continue(size_t* src, PosPtr _dest) : ParserNode(NodeType::CONTINUE), pc(src), dest(_dest) {}
			void execute(char*) override;

		private:
			size_t* pc;
			PosPtr dest;
		};
		class IfFalseJmp : public ParserNode
		{
		public:
			IfFalseJmp(size_t* _pc, ParserNode* _expr) :ParserNode(NodeType::IFTF), pc(_pc), expr(_expr) {}
			void execute(char*) override;
			std::string to_string() const override;
			~IfFalseJmp() { delete expr; }

			PosPtr false_tag;
		private:
			size_t* pc;
			ParserNode* expr;
		};
	}
	namespace Parser
	{
		void build_block();
		void public_part(bool is_single = false);
		void build_for();
		void do_while();
		void build_while();
		void build_if();
	}
}