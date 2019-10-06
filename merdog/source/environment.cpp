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

std::vector<size_t *> Mer::_pcs;
std::string Mer::output_buff = "";
std::vector<std::pair<Mer::PosPtr,Mer::PosPtr>> Mer::_nearest_loop_pos;
std::string Mer::run_interpreter(const std::string&file_content)
{
	output_buff = "";
	try
	{
		Mer::set_io();
		Mer::set_maths();
		Mer::build_token_stream(file_content);
		Mer::Parser::program()->execute();
		token_stream.clear();
//		for (auto &a : del_parser_node_table)
	//		delete a;
	}
	catch (std::exception &e)
	{
		auto ret = e.what();
		token_stream.clear();
		return ret;
	}
	catch (Mer::Error &e)
	{
		auto ret = e.what();
		token_stream.clear();
		return ret;
	}
	return output_buff;
}

void Mer::new_loop(PosPtr a, PosPtr b)
{
	_nearest_loop_pos.push_back(std::make_pair(a,b));
}

void Mer::end_loop()
{
	_nearest_loop_pos.pop_back();
}

Mer::PosPtr & Mer::loop_start()
{
	return _nearest_loop_pos.back().first;
}
Mer::PosPtr & Mer::loop_end()
{
	return _nearest_loop_pos.back().second;
}
