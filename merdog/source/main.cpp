#include <iostream>
#include <time.h>
#include <fstream>
#include "../include/lexer.hpp"
#include "../include/expr.hpp"
#include "../include/parser.hpp"
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
		std::vector<mer::Node> instructions;
		mer::init_merdog();
		std::string input_file = get_file_content("test.mer");
		std::string tmp;
		mer::build_token_stream(input_file);
		while (mer::token_stream.this_tag() != mer::ENDOF)
		{
			instructions.push_back(mer::parse_statement::statement());
		}
		for (auto& a : instructions)
			a->emit_gen();
		for (auto& a : mer::ir_instructions)
		{
			std::cout << a<<std::endl;
		}
	}
	catch (mer::Error &e)
	{
		std::cout << e.what();
	}
	catch (...)
	{
		std::cout << "Damn 未经处理的异常";
	}
	while(1)
		std::cin.get();
	return 0;
}