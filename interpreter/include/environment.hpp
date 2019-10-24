#pragma once
#include "lexer.hpp"
#include <utility>
#include <memory>
namespace Mer
{
	using PosPtr = std::shared_ptr<size_t>;
	extern std::string output_buff;
	std::string run_interpreter(const std::string&file_content);
	//std::vector<ParserNode*> del_parser_node_table;
	extern std::vector<size_t *> _pcs;
	extern std::vector<std::pair<PosPtr, PosPtr>> _nearest_loop_pos;
	void new_loop(PosPtr a,PosPtr b);
	void end_loop();
	PosPtr& loop_start();
	PosPtr& loop_end();
}