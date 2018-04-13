#include <string>
#include <iostream>
#include "../include/parser.hpp"
#include <Windows.h>
#include <fstream>
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
int main()
{
	try
	{
		std::string input_content = get_file_content("test.mer");
		time_t s = clock();
		Mer::build_token_stream(input_content);
		Mer::Parser::program()->execute();
		time_t e = clock();
		std::cout << "\ntime:" << (double)(e - s) / CLK_TCK << std::endl;
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
#ifndef _DEBUG
int main()
{
	std::string str;
	std::cin >> str;
	Mer::build_token_stream(str);
	auto a=Mer::Expr();
	std::cout << a.execute()->to_string();
	Sleep(1000000);
}
#endif