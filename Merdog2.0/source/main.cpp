#include <string>
#include <iostream>
#include "../include/expr.hpp"
#include <Windows.h>
int main()
{
	std::string str;
	std::cin >> str;
	Mer::build_token_stream(str);
	auto a=Mer::Expr();
	std::cout << a.execute()->to_string();
	Sleep(1000000);
}