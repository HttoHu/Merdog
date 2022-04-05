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
        extern std::vector<std::vector<size_t *>> _change_pos_tab;

        size_t *get_dest_pos(ParserNode *node);
        class Goto : public ParserNode
        {
        public:
            Goto(size_t *src, PosPtr _dest) : ParserNode(NodeType::GOTO), pc(src), dest(_dest) {}
            void execute(char *) override;

        private:
            size_t *pc;
            PosPtr dest;
        };
        class IfTrueToAOrB : public ParserNode
        {
        public:
            IfTrueToAOrB(size_t *_pc, PosPtr _true_tag, PosPtr _false_tag, ParserNode *_expr) : ParserNode(NodeType::IFTF),pc(_pc), true_tag(_true_tag), false_tag(_false_tag), expr(_expr) {}
            void execute(char *) override;
            std::string to_string() const override;
            ~IfTrueToAOrB() { delete expr; }

        private:
            size_t *pc;
            PosPtr true_tag;
            PosPtr false_tag;
            ParserNode *expr;
        };
        // push a true value and a size_t to set else pos
        class IfWithJmpTable : public ParserNode
        {
        public:
            IfWithJmpTable(size_t *_pc) :ParserNode(NodeType::IFWTAB), pc(_pc) {}
           void execute(char *) override;
            std::string to_string() const override;
            std::vector<std::pair<UptrPNode, PosPtr>> jmp_table;
            PosPtr end_pos;

        private:
            size_t *pc;
        };
    }
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