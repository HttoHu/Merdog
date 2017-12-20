#include "..\HPP\SystemFunction.hpp"
#include "..\HPP\Maths.hpp"
#include "../HPP/Time.hpp"
using namespace Simcc;
using namespace Simcc::Parser;
std::map < Lexer::Token*, std::function<Runtime::ObjectBase*(Parser::Param*)>> 
System::CPPFunction::system_function_table
{
	{Lexer::TId::find_id("write"),System::console_output},
	{ Lexer::TId::find_id("write_line"),System::console_output_line},
	{ Lexer::TId::find_id("read"),System::console_input},
	{ Lexer::TId::find_id("sqrt"),System::sqrt},
	{Lexer::TId::insert_id("clock"),System::clock}
};

Simcc::System::CPPFunction::CPPFunction()
{
	func_id = Environment::this_token();
	Environment::match(Lexer::Id);
	param = new Param();
	Environment::match(Lexer::EndStmt);
	auto result = system_function_table.find(func_id);
	if (result == system_function_table.end())
		throw std::runtime_error("no found such function");
}

void Simcc::System::CPPFunction::execute()
{
	auto result = system_function_table.find(func_id);
	result->second.operator()(param);
}
