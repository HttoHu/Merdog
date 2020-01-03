/*
	* MIT License
	* Copyright (c) 2019 Htto Hu
*/
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include "../include/parser.hpp"
#include "../include/clib/utility.hpp"
#include "../include/clib/merdog_io.hpp"
#include "../include/clib/maths.hpp"
#include "../include/environment.hpp"
#include "../include/branch_and_loop.hpp"
#include "../include/clib/containers.hpp"
#include "../include/environment.hpp"
#define MERDOG_VERSION "Merdog Snake 1.3.2"
#ifdef _WIN32
#include <Windows.h>
#endif
std::string get_file_content(const std::string& filename)
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
		if (argc != 2)
			throw std::runtime_error("argument error");
		if (sizeof(argv[1]) >= 2)
		{
			if (argv[1][0] == '-' && argv[1][1] == 'v')
			{
				std::cout << MERDOG_VERSION;
				return 0;
			}
		}
		std::string input_content = get_file_content(argv[1]);
		Mer::run_interpreter(input_content);
	}
	catch (std::exception & e)
	{
		std::cout << e.what();
	}
	catch (Mer::Error & e)
	{
		std::cout << e.what();
	}
#ifdef _WIN32
	Sleep(1000);
#endif
	return 0;
}
#else
int main()
{
	try
	{
		time_t s = clock();
		std::string input_content = get_file_content("test.mer");
		Mer::run_interpreter(input_content);
		time_t e = clock();
		std::cout << "\ntime:" << (double)(e - s) / CLK_TCK;
	}
	catch (const std::runtime_error & e)
	{
		std::cout << e.what();
	}
	catch (Mer::Error & e)
	{
		std::cout << e.what();
	}
	catch (...)
	{
		std::cout << "internal error!";
	}
#ifdef _WIN32
	Sleep(10000000);
#endif
	std::cin.get();
	std::cout << "\nThe program has exited";
	return 0;
}
#endif