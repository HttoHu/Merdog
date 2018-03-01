#include "../include/function.hpp"
#include "../include/memory.hpp"
using namespace Mer;
std::map<std::string, size_t> Mer::function_map;

std::vector<FunctionBase*> Mer::function_list;

//==============================================================
Param * Mer::Parser::build_param()
{
	Param *ret = new Param();
	token_stream.match(LPAREN);
	auto test = build_param_part();
	if (test == nullptr)
	{ 
		token_stream.match(RPAREN);
		return ret;
	}
	ret->param.push_back(test);
	while (token_stream.this_tag() == COMMA)
	{
		token_stream.match(COMMA);
		ret->param.push_back(build_param_part());
	}
	token_stream.match(RPAREN);
	return ret;
}

ParamPart * Mer::Parser::build_param_part()
{
	if (token_stream.this_tag() == RPAREN)
		return nullptr;
	auto type = type_spec();
	auto name = token_stream.this_token();
	ParamPart *ret = new ParamPart(type,name );
	token_stream.match(ID);
	return ret;
}

void Mer::Parser::build_function()
{
	Function *func = new Function();
	token_stream.match(FUNCTION);
	auto id = Id::get_value(token_stream.this_token());
	token_stream.match(ID);
	auto test = function_map.find(id);
	if (test != function_map.end())
	{
		auto param = build_param();
		func->param = param->get_param();
		delete param;
		function_list[test->second] = func;
		func->blo = block();
		return;
	}
	function_map.insert({ id,function_list.size() });
	auto param = build_param();
	func->param = param->get_param();
	delete param;
	function_list.push_back(func);
	func->blo = block();
}

Mem::Object Mer::Function::call(std::vector<Mem::Object>& arg,int reserve_size)
{
	try
	{
		_mem.new_func(reserve_size);
		for (size_t i = 0; i < param.size(); i++)
		{
			_mem[param[i]] = arg[i];
		}
		blo->get_value();
	}
	catch (Return *r)
	{
		auto ret = r->get_ret_value();
		_mem.end_func();
		return ret;
	}
	_mem.end_func();
}

std::deque<AST*> Mer::Param::generate_statement(std::vector<Expr*>& v)
{
	std::deque<AST*> ret;
	for (int i = 0; i < v.size(); i++)
	{
		ret.push_back(param[i]->create_var(v[i]));
	}
	return ret;
}

Mer::ParamPart::ParamPart(Type * type, Token * tok)
{
	t = type;
	pos = _mem.push();
	id_pos_table.front().insert({ tok,pos });
}

ParVar * Mer::ParamPart::create_var(Expr *exp)
{
	return new ParVar(pos, exp);
}

Mer::ParVar::ParVar(size_t p, Expr * e)
{
	expr = e;
}

Mem::Object Mer::ParVar::get_value()
{
	_mem[pos] = expr->get_value();
	return _mem[pos];
}