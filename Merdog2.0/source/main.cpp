/*
	* Inspired by
	* https://ruslanspivak.com/lsbasi-part10/
	* Ruslan's Blog
	* C++ Version.
	* Yuantao Hu 2018
	* Email Huyuantao@outlook.com
*/
#include "../include/syslib/system.hpp"
#include "../include/interpreter.hpp"
#include "../include/type.hpp"
#include <string>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <time.h>
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
int main(int argc,char* argv[])
{
	try
	{
		Mer::Sys::import_math();
		Mer::Sys::import_io();
		if (argc != 2)
			throw std::runtime_error("argument error");
		std::string input_content = get_file_content(argv[1]);
		time_t s = clock();
		Mer::build_token_stream(input_content);
		Mer::Interpreter().run();
		time_t e = clock();
		std::cout << (double)(e - s) / CLK_TCK;
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
	catch (Mer::Error &e)
	{
		std::cout << e.what();
	}
	std::cout << "\nThe program has exited";
	Sleep(100000);
	return 0;
}