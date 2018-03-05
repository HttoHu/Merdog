#include "../include/function.hpp"
#include "../include/memory.hpp"
#include "../include/environment.hpp"
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
	symbol_table.insert_basic(name, IdType::TVar);
	symbol_table.insert_type(name, type->get_type());
	ParamPart *ret = new ParamPart(type,name );
	token_stream.match(ID);
	return ret;
}

void Mer::Parser::build_function()
{
	_mem.get_index() = 0;
	token_stream.match(FUNCTION);
	auto type = type_spec();
	Function *func = new Function(type->get_type());
	delete type;
	auto id = Id::get_value(token_stream.this_token());
	symbol_table.push();
	/**do something record**/
	symbol_table.insert_basic(token_stream.this_token(), IdType::TFunction);
	symbol_table.insert_type(token_stream.this_token(), func->get_type());
	token_stream.match(ID);
	auto test = function_map.find(id);
	id_pos_table.push_front(std::map<Mer::Token*, std::size_t>());
	// if the function has been declared.
	if (test != function_map.end())
	{
		auto param = build_param();
		func->param = param->get_param();
		delete param;
		func = static_cast<Function*>(function_list[test->second]);
		func->blo= new Block();
		func->blo->compound_list = static_cast<Compound*>(compound_statement());
		id_pos_table.pop_front();
		symbol_table.pop();
		return;
	}
	//else create a new function;
	function_map.insert({ id,function_list.size() });
	auto param = build_param();
	func->param = param->get_param();
	delete param;
	// to test whether the it is a function.
	if (token_stream.this_tag() == SEMI)
	{
		token_stream.match(SEMI);
		function_list.push_back(func);
		return;
	}
	function_list.push_back(func);
	func->blo = new Block();
	func->blo->compound_list = static_cast<Compound*>(compound_statement());
	// delete id_pos_table
	symbol_table.pop();
	id_pos_table.pop_front();
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