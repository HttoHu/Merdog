#include "..\Parser\HPP\Function.hpp"
#include "../Parser/HPP/Block.hpp"
#include "../Test/HPP/TObject.hpp"
#include "../Runtime/HPP/Run.hpp"
#include <fstream>
#include <time.h>
#include <Windows.h>
#define PT
using namespace Simcc::Runtime;
std::string get_file_content(const std::string & filename);
void block_test()
{
	std::string str = get_file_content("Text.txt");
	Simcc::Lexer::Lexer lex(str);
	lex.init_token_stream();
	lex.debug();
	Simcc::Parser::Environment::token_stream = &lex.token_stream;
	Simcc::Parser::Block block;
	time_t s = clock();
	block.execute();
	std::cout << std::endl<<"====================================="<<std::endl;
	std::cout << (double)(clock()-s)/CLK_TCK<<"\n";
}
void lexer_test()
{
	std::string str = get_file_content("Text.txt");
	Simcc::Lexer::Lexer lex(str);
	lex.init_token_stream();
	lex.debug();
}
void function_test()
{
	std::string str = get_file_content("Text.txt");
	Simcc::Lexer::Lexer lex(str);
	lex.init_token_stream();
	lex.debug();
	std::cout << std::endl;
	Simcc::Parser::Environment::token_stream = &lex.token_stream;
	Simcc::Parser::Function fuc;
	fuc.execute(new Simcc::Parser::Param());
}
#ifndef _DEBUG
int main(int argc,char* argv[])
{
	/* Text.txt ÄÚÈÝ
	*
	{
	int a=10;
	string str="hello world";
	if(str=="hello world")
	--a;
	}
	*/
	try
	{
		if (argc != 2)
			throw std::runtime_error("invaild input");
		Init(argv[1]);
		CreateFunctionTable();
		time_t s = clock();
		Execute();
		std::cin.get();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}
	Sleep(1000000);
	return 0;
}
#endif
#ifndef PT
int main()
{
	/* Text.txt ÄÚÈÝ
	*
	{
	int a=10;
	string str="hello world";
	if(str=="hello world")
	--a;
	}
	*/
	try
	{
		Init("test.sic");
		CreateFunctionTable();
		time_t s = clock();
		Execute();
		std::cin.get();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}
	Sleep(1000000);
	return 0;
}
#endif

int main()
{
	Simcc::Runtime::Basic g(12);
	std::cout << g.to_string();
	std::cin.get();
}