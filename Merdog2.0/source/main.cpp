/*
	* Learned by
	* https://ruslanspivak.com/lsbasi-part10/
	* Ruslan's Blog
	* C++ Version.
*/
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
int main()
{
	try
	{
		std::string input_content = get_file_content("good.pas");
		//std::cout << input_content<<std::endl;
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
int model()
{
	try
	{
		void *b = new int(23);
		std::cout << (b == static_cast<int*>(b));
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
int t1()
{
	try
	{
		{
			using namespace Mer::Mem;
			Int *a = new Int(3);
			Double *d = new Double(2.3);
			time_t s = clock();
			for (int i = 0; i < 100000000; i++)
			{
				delete(*a + d);
			}
			time_t e = clock();
			std::cout << double(e - s) / CLK_TCK<<std::endl;
		}

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