/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/error.hpp"
#include "../include/lexer.hpp"
using namespace mer;
int mer::get_line_no()
{
	auto content = token_stream._get_content();
	auto pos = token_stream._get_pos();
	while (content[pos]->get_tag() != ENDL)
	{
		pos--;
	}
	return Endl::get_value(content[pos]);
}
