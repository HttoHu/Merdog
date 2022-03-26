#include <iostream>
#include <fstream>

#include "../includes/lexer.hpp"
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

int main(){
    bool a = false;
	auto src = get_file_content("test.mer");
	Mer::build_token_stream(src);
	Mer::token_stream.print();
    return 0;
}