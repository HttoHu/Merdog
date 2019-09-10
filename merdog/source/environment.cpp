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

std::string Mer::output_buff = "";
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