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
void test() {
	using namespace std;
	vector<pair<string, string>> vec = { {"LSH","left_shift"},{"RSH","right_shift"}
	,{"EQ","eq"},{"NE","ne"},{"GE","ge"},{"LE","le"},{"GT","gt"}, {"LT","lt"} };
	for (auto [f, s] : vec)
	{
		cout << "{{" + f + ",int_tag,int_tag},{int_tag," + s + "<int_default,int_default,int_default>}},\n";
	}
}
int main() {
	using namespace Mer;
	auto src = get_file_content("test.mer");
	try
	{
		Mer::build_token_stream(src);
		Mer::token_stream.print();
		auto node = Mer::Parser::parse_expr();
		std::cout << node->to_string() << "\n\n";
		std::cout << "\n\nres:\n-----------------------\n";
		node->execute(buf);
		
		
		//std::cout << node->need_space() << "\n";
		//std::cout << *(int_default*)(buf);
	}
	catch (Mer::Error& e)
	{
		std::cout << e.what();
	}

	return 0;
}