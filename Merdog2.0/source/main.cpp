#include "../include/interpreter.hpp"
#include <string>
#include <Windows.h>
#include <iostream>
int main()
{
	std::string input_content;
	getline(std::cin,input_content);
	Mer::build_token_stream(input_content);
    Mer::Interpreter().run();
    std::cout<<"\nThe program have exited";
    std::cin.get();
    std::cin.get();
	Sleep(100000);
    return 0;
}