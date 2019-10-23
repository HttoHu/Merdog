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
}