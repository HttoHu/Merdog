/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/function.hpp"
#include "../include/memory.hpp"
#include "../include/parser.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
#include "../include/environment.hpp"
#include "../include/branch_and_loop.hpp"
#define MER3_1_2
using namespace Mer;

bool Mer::is_struct_member_function=false;
std::map<std::string, Function*> Mer::function_table;
std::map<size_t, FunctionBase*> Mer::type_init_function_map;
Block *Mer::current_function_block = nullptr;
std::map<size_t, Mem::Object> Mer::type_init_map;
//=============================================================
bool	is_function_statement()
{
	int index = 1;
	if (token_stream.this_tag() == LPAREN)
	{
		while (token_stream.this_token(index)->get_tag() != RPAREN)
		{
			if (token_stream.this_token(index) == END_TOKEN)
				throw Error("Reached end of the file");
			index++;
		}
		if (token_stream.this_token(size_t(index) + 1)->get_tag() == SEMI)
		{
			return true;
		}
		else
			return false;
	}
	else
		throw Error("it isn't a function");
}

ParamFeature Mer::Parser::build_param_feature()
{
	ParamFeature ret;
	token_stream.match(LPAREN);
	if (is_struct_member_function)
	{
		ret.push_back((size_t)(Mem::type_counter) + 1);
		is_struct_member_function = false;
	}
	if (token_stream.this_tag() == RPAREN)
	{
		token_stream.match(RPAREN);
		return ret;
	}
	//the first arg of member function is the obj of the structure, 
	while (true)
	{
		size_t type = Mem::get_type_code();
		ESymbol es = SVAR;
		if (token_stream.this_tag() == MUL)
		{
			token_stream.next();
			type++;
		}
		token_stream.match(ID);
		ret.push_back(type);
		if (token_stream.this_tag() == COMMA)
			token_stream.match(COMMA);
		else
			break;
	}
	token_stream.match(RPAREN);
	return ret;
}

Param * Mer::Parser::build_param()
{
	Param *ret = new Param();
	token_stream.match(LPAREN);
	// 	the first arg of member function is the obj of the structure, 
	if (is_struct_member_function)
	{
		ret->push_new_param((size_t)(Mem::type_counter), mem.push());
		is_struct_member_function = false;
	}
	if (token_stream.this_tag() == RPAREN)
	{
		token_stream.match(RPAREN);
		return ret;
	}
	while (true)
	{
		size_t type = Mem::get_type_code();
		ESymbol es = SVAR;
		if (token_stream.this_tag() == MUL)
		{
			token_stream.next();
			type++;
		}
		auto name = new NamePart();

		size_t pos = mem.push(name->get_count())-1;
		tsymbol_table->push(Id::get_value(name->get_id()), new VarIdRecorder(type, pos,es));
		ret->push_new_param(type, pos);
		if (token_stream.this_tag() == COMMA)
			token_stream.match(COMMA);
		else
			break;
	}
	token_stream.match(RPAREN);
	return ret;
}
std::pair<std::string, Function*> Parser::_build_function()
{
	using namespace Mer;
	using namespace Parser;
	token_stream.match(FUNCTION);
	size_t rtype = Mem::get_type_code();
	// if the ret type is a pointer

	if (token_stream.this_tag() == MUL)
	{
		token_stream.next();
		rtype++;
	}
	current_function_rety = rtype;
	this_namespace->sl_table->new_block();
	std::string name = Id::get_value(token_stream.this_token());
	token_stream.next();

	mem.new_block();
	Param* param = build_param();
	Function* ret = new Function(rtype, param);
	Mer::global_stmt() = false;
	_pcs.push_back(ret->pc);
	// bind function block to it, and when you call build_function_block, the function block will be built.
	current_ins_table = &(ret->stmts);
	int off = Parser::build_function_block() + ret->param->get_param_table().size();
	// continue to tag other statement to global stmt.
	Mer::global_stmt() = true;

	ret->is_completed = true;
	ret->set_index(off);
	return { name,ret };
}

void Mer::Parser::build_function()
{
	token_stream.match(FUNCTION);
	size_t rtype = Mem::get_type_code();
	// if the ret type is a pointer
	if (token_stream.this_tag() == MUL)
	{
		token_stream.next();
		rtype++;
	}
	current_function_rety = rtype;
	this_namespace->sl_table->new_block();
	std::string name = Id::get_value(token_stream.this_token());
	token_stream.next();
	// if the function has decleared.
	auto finder = this_namespace->functions.find(name);
	if (finder != this_namespace->functions.end())
	{
		// the type of finder->second is FunctionBase.
		if (finder->second->is_completed == true)
		{
			throw Error("function redefined.");
		}
		// create a function and return it.
		Function *temp = static_cast<Function*>(finder->second);
		mem.new_block();
		temp->param = build_param();
		// we use pure_block because we should push the param to the block, 
		// so we need to create a preserved memory for param.
		Mer::global_stmt() = false;

		_pcs.push_back(temp->pc);
		current_ins_table = &(temp->stmts);
		// off get the var_size of the function
		int off=Parser::build_function_block()+ temp->param->get_param_table().size();
		Mer::global_stmt() = true;
		temp->is_completed = true;
		temp->set_index(off);
		return;
	}
	this_namespace->sl_table->push_glo(name, new FuncIdRecorder(rtype));
	if (is_function_statement())
	{
		Function*ret = new Function(rtype);
		auto param_types = build_param_feature();
		token_stream.match(SEMI);
		ret->set_param_types(param_types);
		this_namespace->functions.insert({ name,ret });
		ret->is_completed = false;
		return;
	}
	// create a function and return it.
	mem.new_block();
	Param *param = build_param();
	Function *ret = new Function(rtype, param);
	this_namespace->functions.insert({ name,ret });
	Mer::global_stmt() = false;
	_pcs.push_back(ret->pc);
	current_ins_table = &(ret->stmts);
	// off get the var_size of the function
	int off=Parser::build_function_block() + ret->param->get_param_table().size();
	Mer::global_stmt() = true;
	ret->is_completed = true;
	ret->set_index(off);
}

bool Mer::Param::type_check(const std::vector<size_t>& types)
{
	if (types.size() != arg_pos.size())
	{
		return false;
	}
	for (size_t i = 0; i < types.size(); i++)
	{
		auto type_seeker = Mem::type_map.find(types[i]);
		if (type_seeker == Mem::type_map.end())
		{
			throw Error("<inner error>type were no found");
		}
		if (!type_seeker->second->convertible(arg_pos[i].second))
		{
			return false;
		}
	}
	return true;
}

Mer::FunctionBase::FunctionBase()
{
}

void Mer::FunctionBase::check_param(const std::vector<size_t>& types)
{
	if (types.size() != param_types.size())
	{
		throw Error("A03 argument size error expect "+std::to_string(param_types.size())+" but receive "+std::to_string(types.size()));

	}
	for (size_t i = 0; i < param_types.size(); i++)
	{
		if (types[i] == param_types[i])
		{
			continue ;
		}
		auto type_seeker = Mem::type_map.find(types[i]);
		if (type_seeker == Mem::type_map.end())
		{
			throw Error("A01 exists an undefined type : type code "+std::to_string(types[i]));
		}
		if (!type_seeker->second->convertible(param_types[i]))
		{
			throw Error("A02 Error: type not matched type: " + Mem::type_to_string(Mem::BasicType(param_types[i])) + " to " + Mem::type_to_string(Mem::BasicType(types[i])));
		}
	}
	return;
}

void Mer::FunctionBase::convert_arg(std::vector<ParserNode*>& args)
{
	// ensure you have checked the args' type.
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i]->get_type() != param_types[i])
		{
			Expr *tmp=new ImplicitConvertion(param_types[i]);
			tmp->tree = args[i];
			args[i] = tmp;
		}
	}
}
//================================================================
void Mer::FunctionBase::set_index(size_t pos)
{
	function_offset = (int)pos;
}

Mer::Function::Function(size_t t, Param * p) :
	type(t), param(p)
{
	for (const auto &a : param->get_param_table())
	{
		param_types.push_back(a.first);
	}
}

Mer::Function::Function(size_t t) :type(t) {}


void Mer::Function::reser_param(Param * p)
{
	for (const auto &a : param->get_param_table())
	{
		param_types.push_back(a.first);
	}
}

Mem::Object Mer::Function::run(std::vector<Mem::Object>& objs)
{
	mem.new_func(function_offset);
	auto param_table = param->get_param_table();
	for (size_t i = 0; i < param->get_param_table().size(); i++)
	{
		mem[mem.get_current()+param_table[i].second] = objs[i];
	}
	size_t tmp = *pc;
	for (*pc = 0; *pc < stmts.size(); ++ * pc)
		stmts[*pc]->execute();
	*pc = tmp;
	mem.end_func();
	return function_ret;
}

void Mer::Function::set_function_block()
{

}

void Mer::SystemFunction::check_param(const std::vector<size_t>& types)
{
	if (check_param_type)
		FunctionBase::check_param(types);
}