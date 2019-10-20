#include "..\include\parser_node.hpp"
namespace mer
{
	std::string mer::ParserNode::get_gen()
	{
		return "empty node";
	}
	std::string ParserNode::to_string()
	{
		 return "empty node";
	}
	instruction ParserNode::get_instruction()
	{
		return nullptr;
	}
	size_t ParserNode::get_type()
	{
		return 0;
	}
}