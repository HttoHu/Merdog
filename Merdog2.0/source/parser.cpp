#include "../include/parser.hpp"
#include "../include/memory.hpp"
using namespace Mer;

Program* Mer::Parser::program()
{	
	token_stream.match(PROGRAM);
	auto tmp = token_stream.this_token();
	token_stream.match(ID);
	std::string name = Id::get_value(tmp);
	stack_memory.new_block();
	auto blo = block();
	stack_memory.end_block();
	token_stream.match(DOT);
	return new Program(tmp, blo);
}

Block * Mer::Parser::block()
{
	return nullptr;
}
