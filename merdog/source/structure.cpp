#include "..\include\structure.hpp"
#include "../include/namespace.hpp"
#include "../include/word_record.hpp"
std::map<size_t, Mer::StructureBase*> Mer::structure_seeker;
//Mer::SComVarFuncCall::SComVarFuncCall(const std::vector<size_t>& types, FunctionBase * fun, std::vector<Expr*>& exprs){}

Mer::ParserNode * Mer::Parser::parse_compound_var_id()
{
	auto id_token = token_stream.this_token();
	auto id = tsymbol_table->find(Mer::String::get_value(token_stream.this_token()));
	token_stream.match(ID);
	StructureBase * structure= nullptr;
	{
		// find structure;
		auto result = structure_seeker.find(id->get_type());
		if (result == structure_seeker.end())
			throw Error("type no found");
		structure = result->second;
	}
	switch (token_stream.this_tag())
	{
	case DOT:
	{
		token_stream.match(DOT);
		ParserNode* tmp = new Variable(id_token);
		return parse_function_call(static_cast<Expr*>(tmp), structure);
	}
	default:
		break;
	}
	return new Variable(id_token);
}
