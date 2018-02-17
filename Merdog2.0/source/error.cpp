#include "../include/error.hpp"
#include "../include/lexer.hpp"
using namespace Mer;
size_t Mer::get_line_no()
{
	auto content = token_stream._get_content();
	auto pos = token_stream._get_pos();
	while (content[pos]->get_tag() != ENDL)
	{
		pos--;
	}
	return Endl::get_value(content[pos]);
}