#include "..\include\parser_node.hpp"
namespace mer
{
	std::string mer::ParserNode::get_gen()
	{
		return "";
	}
	std::string ParserNode::to_string()
	{
		 return "empty node";
	}
	Type* ParserNode::get_type()
	{
		return type;
	}
}