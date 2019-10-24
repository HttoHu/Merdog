#include "../include/id_and_value.hpp"
#include "../include/symbol_table.hpp"
namespace mer
{
	extern std::map<std::string, Type*> type_map;
	std::string get_tmp_var_name(bool);
	IntV::IntV(int v) :value(v), ParserNode(ParserNode::CINTV)
	{
		set_type(type_map["int"]);
	}
	IntV::IntV() : ParserNode(ParserNode::CINTV)
	{
		value = Integer::get_value(token_stream.this_token());
		token_stream.next();
		set_type(type_map["int"]);

	}
	std::string IntV::to_string()
	{
		return "i" + std::to_string(value);
	}
	Variable::Variable(std::string n):ParserNode(VARIABLE),name(n)
	{
		auto result = find_recorder_by_id(n);
		set_type(result->type);
	}
	RealV::RealV(double v):value(v), ParserNode(ParserNode::REAL_VALUE)
	{
		set_type(type_map["real"]);
	}
	RealV::RealV():ParserNode(ParserNode::REAL_VALUE)
	{
		value = Real::get_value(token_stream.this_token());
		token_stream.next();
		set_type(type_map["real"]);
	}
	std::string RealV::to_string()
	{
		return "f"+std::to_string(value);
	}
	BoolV::BoolV(bool v) :ParserNode(ParserNode::BOOL_VALUE) ,value(v)
	{
		set_type(type_map["bool"]);
	}
	BoolV::BoolV() :ParserNode(ParserNode::BOOL_VALUE)
	{
		value = token_stream.this_tag() == TTRUE;
		token_stream.next();
		set_type(type_map["bool"]);
	}
	CharV::CharV(char v):ParserNode(ParserNode::CHAR_VALUE),value(v)
	{
		set_type(type_map["char"]);
	}
	CharV::CharV():ParserNode(ParserNode::CHAR_VALUE){
		value = Char::get_value(token_stream.this_token());
		token_stream.next();
		set_type(type_map["char"]);
	}
}