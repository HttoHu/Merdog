#include "../include/object.hpp"
#include "../include/expression.hpp"
#include <time.h>
#include <iostream>
#include <fstream>
std::string get_file_content(const std::string & filename)
{
	using namespace std;
	ifstream ifs(filename);
	std::string file_content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	return file_content;
}
int main()
{
	using namespace Merdog;
	try
	{
		time_t start = clock();
		
		Environment::file_content() = "a*b*c+3;";
		scan();
		//Environment::show_ts();


		Expr()._show();
		time_t end = clock();
		std::cout << "\ntime:" << (double)(end - start) / CLK_TCK;
	}
	catch (const std::exception &e)
	{
		std::cout << e.what();
	}
	std::cin.get();
}
