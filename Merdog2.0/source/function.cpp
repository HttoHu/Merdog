#include "../include/function.hpp"
#include "../include/var.hpp"
#include "../include/memory.hpp"
#include "../include/parser.hpp"
#include "../include/word_record.hpp"
using namespace Mer; 
std::map<std::string, Function*> Mer::function_table;
Param * Mer::Parser::build_param()
{
	Param *ret = new Param();
	token_stream.match(LPAREN);
	while (true)
	{
		size_t type = Mem::get_type_code(token_stream.this_token());
		token_stream.next();
		auto name = String::get_value(token_stream.this_token());
		token_stream.match(ID);
		size_t pos = stack_memory.push();
		global_symbol_table.push(name, new VarIdRecorder(type, pos));
		ret->push_new_param(type, pos);
		if (token_stream.this_tag() == COMMA)
			token_stream.match(COMMA);
		else
			break;
	}
	token_stream.match(RPAREN);
	return ret;
}
void Mer::Parser::build_function()
{
	token_stream.match(FUNCTION);
	size_t rtype = Mem::get_type_code(token_stream.this_token());
	token_stream.next();
	stack_memory.new_block();
	global_symbol_table.new_block();
	auto name = Id::get_value(token_stream.this_token());
	token_stream.next();
	global_symbol_table.push(name, new FuncIdRecorder(rtype));
	Param *param = build_param();
	Block *blo = pure_block();
	Function*ret = new Function(rtype, param, blo);
	function_table.insert({ name,ret });
}

bool Mer::Param::type_check(const std::vector<size_t>& types)
{
	if (types.size() != param_pos.size())
		return false;
	for (size_t i = 0; i < types.size(); i++)
	{
		if (types[i] != param_pos[i].first)
			return false;
	}
	return true;
}

Mem::Object Mer::Function::run(const std::vector<Expr*> exprs)
{
	stack_memory.new_func(param->get_param_table().size());
	for (size_t i = 0; i < param->get_param_table().size(); i++)
	{
		stack_memory[param->get_param_table()[i].second] = exprs[i]->execute();
	}
	try
	{
		blo->execute();
	}
	catch (Return *r)
	{
		auto ret = r->get_expr()->execute();
		stack_memory.end_func();
		return ret;
	}
}
