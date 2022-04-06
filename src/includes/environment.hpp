#pragma once 
#include "./symbols.hpp"
#include <vector>

namespace Mer {
	class ParserNode;
    using PosPtr = std::shared_ptr<size_t>;
	namespace Env {
		

		extern Symbol::SymbleTable* symbol_table;

		// function block,every function has a ins tab and a pc
		// break and continue return can change the pc
		extern std::vector<UptrPNode>* cur_ins_table;
		// every function 
		extern size_t* cur_pc;
        // pos start and end
        extern std::vector<std::pair<PosPtr,PosPtr>> nearest_loop_pos;
		// in 'funciton' or 'for' etc. their var decls should defined in their block.
		extern bool skip_block_begin;

        void new_loop(PosPtr start,PosPtr end);
        void end_loop();
        PosPtr loop_start();
        PosPtr loop_end();
		void new_block();
		void end_block();
	}

	void run_interpreter(const std::string& content);

}