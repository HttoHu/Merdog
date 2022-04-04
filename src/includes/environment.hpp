#pragma once 
#include "./symbols.hpp"
#include <vector>

namespace Mer {
	class ParserNode;

	namespace Env {
		using PosPtr = std::unique_ptr<size_t>;

		extern Symbol::SymbleTable symbol_table;

		// function block,every function has a ins tab and a pc
		// break and continue return can change the pc
		extern std::vector<UptrPNode> cur_ins_table;
		// every function 
		extern size_t* cur_pc;
	}

	void run_interpreter(const std::string& content);

}