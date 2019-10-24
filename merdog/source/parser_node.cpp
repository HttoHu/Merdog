#include "../include/parser_node.hpp"
#include "../include/type.hpp"
namespace mer
{
	extern std::string get_tmp_var_name(bool =false);
	extern std::vector<std::string> ir_instructions;
	void mer::ParserNode::emit_gen()
	{
	}
	std::string ParserNode::to_string()
	{
		 return "empty node";
	}
	Type* ParserNode::get_type()
	{
		return type;
	}
	std::string type_convert(Type* s, Type* dest, Node n)
	{
		std::string name = get_tmp_var_name();
		std::string ins;
		ins += dest->name()+" "+name+"="+"("+dest->name()+")"+n->to_string();
		ir_instructions.push_back(ins);
		return name;
	}
}