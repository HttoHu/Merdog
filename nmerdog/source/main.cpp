#include <iostream>
#include <time.h>
#include "../include/instructions.hpp"
/*
45 F8 03 00 00 00 mov         dword ptr [a],3
	 5: 	a = 34;
0062346F C7 45 F8 22 00 00 00 mov         dword ptr [a],22h
*/
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