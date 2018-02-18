#include "..\HPP\Run.hpp"
#include <fstream>
std::string get_file_content(const std::string & filename)
{
	using namespace std;
	ifstream ifs(filename);
	std::string file_content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	return file_content;
}
void Simcc::Runtime::Init(const std::string & str)
{
	std::string content = get_file_content(str);
	static Simcc::Lexer::Lexer lex(content);
	lex.init_token_stream();
	Parser::Environment::token_stream = &lex.token_stream;
}

void Simcc::Runtime::CreateFunctionTable()
{
	while (Parser::Environment::current_pos < Parser::Environment::token_stream->size()-1)
	{ 
		Parser::Function *func = new Parser::Function();
	}
}

void Simcc::Runtime::Execute()
{
	 auto loadfunc = Parser::Function::function_table.find(Lexer::TId::find_id("main"));
	 if (loadfunc != Parser::Function::function_table.end())
		 loadfunc->second->execute();
	 else
		 throw std::runtime_error("no found main ");
}
