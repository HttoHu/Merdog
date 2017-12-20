#include "..\HPP\Single.hpp"
#include "../../Runtime/HPP/Action.hpp"
using namespace Simcc;
Lexer::Token * Simcc::Parser::Single::next_token()
{
	return Environment::token_stream->operator[](Environment::current_pos++);
}

bool Simcc::Parser::Single::match(Lexer::Tag t)
{
	if ((Environment::token_stream)->operator[](Environment::current_pos++)->get_tag() == t)
	{
		return true;
	}
	return false;
}

Simcc::Parser::ActionStmt::ActionStmt()
{
	action = new Simcc::Runtime::Action();
	Environment::match(Lexer::EndStmt);
}

void Simcc::Parser::ActionStmt::execute() {
	action->execute();
}