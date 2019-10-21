#include "expr.hpp"
#include "type.hpp"
namespace mer
{
	std::string get_tmp_var_name(bool clear = false);
	IntV::IntV(int v) :value(v), ParserNode(ParserNode::CINTV) 
	{
		set_type(type_map["int"] );
	}
	std::string IntV::get_gen()
	{
		return "int" + get_tmp_var_name()+"="+std::to_string(value)+"\n";
	}
	std::string BinOp::get_gen()
	{
		std::string ret = left->get_gen();
	}
}
