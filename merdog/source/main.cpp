#include <string>
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <time.h>
#include "../include/parser.hpp"
#include "../include/clib/merdog_io.hpp"
#include "../include/clib/maths.hpp"
#include "../include/environment.hpp"
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
/*
int main(int argc, char* argv[])
{
	try
	{
		Mer::set_io();
		Mer::set_maths();
		if (argc != 2)
			throw std::runtime_error("argument error");
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
*/
int main()
{
	std::cout<<Mer::run_interpreter(get_file_content("test.mer"));
	/*
	try
	{
		Mer::set_io();
		Mer::set_maths();
		std::string input_content = get_file_content("test.mer");
		time_t s = clock();
		Mer::build_token_stream(input_content);
		Mer::Parser::program()->execute();
		time_t e = clock();
		std::cout << "\ntime:" << (double)(e - s) / CLK_TCK;
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
	catch (Mer::Error &e)
	{
		std::cout << e.what();
	}
	std::cout << "\nThe program has exited";*/
	Sleep(100000);
	return 0;
}