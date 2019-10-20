#include <iostream>
#include <time.h>
#include <fstream>
#include "../include/instructions.hpp"

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
	char ad = 32;
	char bd = 0;
	using namespace mer;
	ins_table.push_back({ ins::push,{sizeof(char),32,0x00} });
	ins_table.push_back({ ins::add<char>,{0x0,0x0,0x1} });
	ins_table.push_back({ ins::exit,{0x0,0x00,0x0} });

	run();

	std::cout << (int)data_cast<char>(1)<<std::endl;
	std::cin.get();
	return 0;
}