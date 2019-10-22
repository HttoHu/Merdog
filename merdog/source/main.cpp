#include <iostream>
#include <time.h>
#include <fstream>
#include "../include/lexer.hpp"
#include "../include/expr.hpp"
#include "../include/environment.hpp"
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
int main()
{
	try
	{
		mer::init_merdog();
		std::string input_file = get_file_content("test.mer");
		mer::build_token_stream(input_file);
		mer::Node tmp = mer::analyse_expr::expr();
		//mer::token_stream.print();
		std::cout << tmp->get_gen();
	}
	catch (mer::Error &e)
	{
		std::cout << e.what();
	}
	catch (...)
	{
		std::cout << "Damn 未经处理的异常";
	}
	std::cin.get();
	return 0;
}