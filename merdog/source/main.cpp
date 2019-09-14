/*
	* MIT License
	* Copyright (c) 2019 Htto Hu
*/
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include "../include/parser.hpp"
#include "../include/clib/merdog_io.hpp"
#include "../include/clib/maths.hpp"
#include "../include/environment.hpp"
#include <Windows.h>
std::string get_file_content(const std::string & filename)
{
	using namespace std;
	ifstream ifs(filename);
	if (!ifs)
	{
		throw std::runtime_error("invalid file");
	}
	std::string file_content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	return file_content;
}

#ifndef _DEBUG
int main(int argc, char* argv[])
{
	try
	{
		Mer::set_io();
		Mer::set_maths();
		if (argc != 2)
			throw std::runtime_error("argument error");
		if (argv[1] == "-v")
		{
			std::cout << "3.0";
			return 0;
		}
		std::string input_content = get_file_content(argv[1]);
		Mer::build_token_stream(input_content);
		Mer::Parser::program()->execute();
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
	catch (Mer::Error &e)
	{
		std::cout << e.what();
	}
	Sleep(100000);
	return 0;
}
#else
int main()
{
	try
	{
		Mer::set_io();
		Mer::set_maths();
		std::string input_content = get_file_content("test.mer");
		Mer::build_token_stream(input_content);
		time_t s = clock();
		Mer::Parser::program()->execute();
		time_t e = clock();
		std::cout << "\ntime:" << (double)(e - s)*1000 / CLK_TCK <<" ms";
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
	catch (Mer::Error &e)
	{
		std::cout << e.what();
	}
	catch (...)
	{
		std::cout << "internal error!";
	}
	std::cout << "\nThe program has exited";
	Sleep(100000);
	return 0;
}
#endif