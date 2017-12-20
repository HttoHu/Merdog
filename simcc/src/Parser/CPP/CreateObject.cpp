#include "..\HPP\CreateObject.hpp"
using namespace Simcc::Parser;
using namespace Simcc;
void Simcc::Parser::CreateBasicTypeObject::execute()
{
	for (auto & a : vtable)
	{
		Environment::stack_block.push(a.first, a.second->GetResult());
	}
}

Lexer::Token * Simcc::Parser::CreateBasicTypeObject::next_token()
{
	return Environment::token_stream->operator[](Environment::current_pos++);
}
Lexer::Token * Simcc::Parser::CreateBasicTypeObject::this_token()
{
	return Environment::token_stream->operator[](Environment::current_pos);
}
