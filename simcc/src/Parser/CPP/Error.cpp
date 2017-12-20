#include "..\HPP\Error.hpp"
#include "../HPP/Environment.hpp"
char const * Simcc::Error::SyntaxError::what() const
{
	size_t pos = Simcc::Parser::Environment::current_pos;
	while (Simcc::Parser::Environment::token_stream->at(--pos)->get_tag() != Lexer::Endl)
	{
	}
	size_t lines = *(size_t*)Simcc::Parser::Environment::token_stream->at(pos)->get_value();
	std::string ret = "\nline:" + std::to_string(lines)+ "\t" + error_content;
	return ret.c_str();
}