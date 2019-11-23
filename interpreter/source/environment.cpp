/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include "../include/environment.hpp"
#include "../include/parser.hpp"
#include "../include/clib/maths.hpp"
#include "../include/clib/containers.hpp"
#include "../include/clib/dictionary.hpp"
std::map<std::string, void(*)()> Mer::repository{
	{"vector",Mer::Container::using_vector},{"deque",Mer::Container::using_deque},
	{"set",Mer::using_set},{"map",Mer::using_map}
};
std::vector<size_t*> Mer::_pcs;
std::string Mer::output_buff = "";
std::vector<std::pair<Mer::PosPtr, Mer::PosPtr>> Mer::_nearest_loop_pos;
std::vector<Mer::ParserNode*> Mer::structure_parent_stack;
void clear()
{
	Mer::token_stream.clear();
	Mer::function_table.clear();
	delete Mer::root_namespace;
	Mer::root_namespace = new Mer::Namespace(nullptr);
	Mer::this_namespace = Mer::root_namespace;
	Mer::function_table.clear();
}
void _merdog_init_()
{
	using namespace Mer;
	using namespace Mem;
	auto compare_op = [](Mem::Object a, Mem::Object b) {
		return std::static_pointer_cast<Mem::Bool>(a->operator<(b))->_value(); };
	compare_map.insert({ Mem::INT,compare_op });
	compare_map.insert({ Mem::BOOL,compare_op });
	compare_map.insert({ Mem::DOUBLE,compare_op });
	compare_map.insert({ Mem::STRING,compare_op });
	compare_map.insert({ Mem::CHAR,compare_op });
	Mer::set_io();
	Mer::set_maths();
}
std::string Mer::run_interpreter(const std::string& file_content)
{
	static int run_time = 0;
	output_buff = "";
	if (!run_time)
		_merdog_init_();
	try
	{
		run_time++;
		Mer::build_token_stream(file_content);
		Mer::Parser::program()->execute();

		token_stream.clear();
		clear();
	}
	catch (std::exception & e)
	{
		auto ret = e.what();
		token_stream.clear();
		clear();
		return ret;
	}
	catch (Mer::Error & e)
	{
		auto ret = e.what();
		token_stream.clear();
		clear();
		return ret;
	}
	return output_buff;
}

void Mer::new_loop(PosPtr a, PosPtr b)
{
	_nearest_loop_pos.push_back(std::make_pair(a, b));
}

void Mer::end_loop()
{
	_nearest_loop_pos.pop_back();
}

Mer::PosPtr& Mer::loop_start()
{
	return _nearest_loop_pos.back().first;
}
Mer::PosPtr& Mer::loop_end()
{
	return _nearest_loop_pos.back().second;
}
