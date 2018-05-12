#include "../include/function.hpp"
#include "../include/var.hpp"
#include "../include/memory.hpp"
#include "../include/parser.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
using namespace Mer; 
std::map<std::string, Function*> Mer::function_table;
size_t Mer::this_func_type;

Param * Mer::Parser::build_param()
{
	Param *ret = new Param();
	token_stream.match(LPAREN);
	if (token_stream.this_tag() == RPAREN)
	{
		token_stream.match(RPAREN);
		return ret;
	}
	while (true)
	{
		size_t type = Mem::get_type_code(token_stream.this_token());
		token_stream.next();
		auto name = Id::get_value(token_stream.this_token());
		token_stream.match(ID);
		size_t pos = stack_memory.push();
		this_namespace->sl_table->push(name, new VarIdRecorder(type, pos));
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
	this_func_type = rtype;
	token_stream.next();
	stack_memory.new_block();
	this_namespace->sl_table->new_block();
	auto name = Id::get_value(token_stream.this_token());
	token_stream.next();
	this_namespace->sl_table->push_glo(name, new FuncIdRecorder(rtype));
	Param *param = build_param();
	Function*ret = new Function(rtype, param);
	this_namespace->functions.insert({ name,ret });
	Block *blo = pure_block();
	ret->reset_block(blo);
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

Mem::Object Mer::Function::run(std::vector<Mem::Object>& objs)
{
	stack_memory.new_func(param->get_param_table().size());
	for (size_t i = 0; i < param->get_param_table().size(); i++)
	{
		stack_memory[param->get_param_table()[i].second] = objs[i];
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
	stack_memory.end_func();
	return nullptr;
}
