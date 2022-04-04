#pragma once

#include <string>
#include <vector>
#include "./parser_node.hpp"

namespace Mer
{
	// goto ,break and some other statements may need a loop end position. so when parsing loop over, 
	// you need to change their value, wo push the unchanged position to a table and update it when the loop parsing over.
	namespace BranchAndLoop {
		extern std::vector<std::vector<size_t*>> _change_pos_tab;

		size_t* get_dest_pos(ParserNode* node);
			class Goto :public ParserNode {
			public:
				Goto(size_t* src) :ParserNode(NodeType::GOTO), pc(src), dest(0) {}
				void execute(char*)override;

			private:
				size_t* pc;
				size_t dest;
		};
		class IfTrueOrFalse :public ParserNode {
		public:
			IfTrueOrFalse(size_t* _pc);
		private:
			size_t* pc;
			size_t true_pos;
			size_t false_pos;
			ParserNode* expr;
		};
	}

}