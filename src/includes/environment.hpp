#pragma once 
#include "./symbols.hpp"

namespace Mer {
	namespace Env {
		extern Symbol::SymbleTable symbol_table;
	}

	void run_interpreter(const std::string& content);

}