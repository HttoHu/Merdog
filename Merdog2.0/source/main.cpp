/*
	* inspired by 
	* https://ruslanspivak.com/lsbasi-part9/
	* Ruslan's Blog
*/
#include "../include/interpreter.hpp"
#include <string>
#include <Windows.h>
#include <fstream>
#include <iostream>
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
		std::string input_content = get_file_content("good.mer");
		std::cout << input_content;
		Mer::build_token_stream(input_content);
		Mer::Interpreter().run();
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
	std::cout << "\nThe program has exited";
	std::cin.get();
	std::cin.get();
	Sleep(100000);
	return 0;
}