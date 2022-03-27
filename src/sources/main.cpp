#include <iostream>
#include <fstream>
#include <string>

#include "../includes/lexer.hpp"
#include "../includes/error.hpp"
#include "../includes/expr.hpp"

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
char buf[512];
int main() {
	using namespace Mer;
	auto src = get_file_content("test.mer");
	try
	{
		Mer::build_token_stream(src);
		Mer::token_stream.print();
		auto node = Mer::Parser::parse_expr();
		node->execute(buf);
		std::cout << node->to_string() << "\n\n";
		std::cout << node->need_space() << "\n";
		std::cout << *(int_default*)(buf);
	}
	catch (Mer::Error& e)
	{
		std::cout << e.what();
	}

	return 0;
}