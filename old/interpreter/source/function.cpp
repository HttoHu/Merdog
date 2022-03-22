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
using namespace Mer;

std::map<type_code_index, _compare_operator> Mer::compare_map;
std::set<FunctionBase*> Mer::rem_functions;
std::map<InitKey, FunctionBase*> Mer::type_init_function_map;
std::map<type_code_index, Mem::Object> Mer::type_init_map;
//=============================================================
bool	is_function_statement()
{
	size_t index = 1;
	if (token_stream.this_tag() == LPAREN)
	{
		while (token_stream.this_token(index)->get_tag() != RPAREN)
		{
			if (token_stream.this_token(index) == END_TOKEN)
				throw Error("Reached end of the file");
			index++;
		}
		if (token_stream.this_token(index + 1)->get_tag() == SEMI)
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

	if (token_stream.this_tag() == RPAREN)
	{
		token_stream.match(RPAREN);
		return ret;
	}
	//the first arg of member function is the obj of the structure, 
	while (true)
	{
		type_code_index type = Mem::get_type_code();
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

Param* Mer::Parser::build_param()
{
	Param* ret = new Param();
	token_stream.match(LPAREN);
	// 	the first arg of member function is the obj of the structure, 

	if (token_stream.this_tag() == RPAREN)
	{
		token_stream.match(RPAREN);
		return ret;
	}
	while (true)
	{
		type_code_index type = Mem::get_type_code();
		ESymbol es = SVAR;
		if (token_stream.this_tag() == MUL)
		{
			token_stream.next();
			type++;
		}
		auto name = new NamePart();

		size_t pos = mem.push(name->get_count()) - 1u;
		tsymbol_table->push(Id::get_value(name->get_id()), new VarIdRecorder(type, pos, es));
		ret->push_new_param(type, pos);
		delete name;
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
	type_code_index rtype = Mem::get_type_code();
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
	// new block new pc;
	_pcs.push_back(ret->pc);
	// bind function block to it, and when you call build_function_block, the function block will be built.
	current_ins_table = &(ret->stmts);
	Parser::build_function_block();

	// count function size;
	ret->off = mem.function_block_size;
	mem.reset_func_block_size();

	_pcs.pop_back();
	// continue to tag other statement to global stmt.
	Mer::global_stmt() = true;

	ret->is_completed = true;
	return { name,ret };
}

void Mer::Parser::build_function()
{
	token_stream.match(FUNCTION);
	type_code_index rtype = Mem::get_type_code();
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
	auto param = build_param();
	auto param_feature = param->get_param_feature();
	if (token_stream.this_tag() == SEMI)
	{
		mem.end_block();
		Function* ret = new Function(rtype,param);
		token_stream.match(SEMI);
		this_namespace->set_new_func(name, ret);
		ret->is_completed = false;
		return;
	}

	auto finder = this_namespace->sl_table->find(name);
	FuncIdRecorder* func_recorder = nullptr;
	// if the function has decleared. finish it
	if (finder != nullptr)
	{
		if (finder->es != SFUN)
			throw Error("id " + name + " redefined");
		else
			func_recorder = static_cast<FuncIdRecorder*>(finder);

		auto func = func_recorder->find(param_feature);
		if (func != nullptr&&func->is_completed==false)
		{
			// create a function and return it.
			Function* temp = static_cast<Function*>(func);

			Mer::global_stmt() = false;
			_pcs.push_back(temp->pc);
			current_ins_table = &(temp->stmts);
			// off get the var_size of the function
			build_function_block();
			// count the block size;
			temp->off = mem.function_block_size;
			mem.reset_func_block_size();

			_pcs.pop_back();
			Mer::global_stmt() = true;
			temp->is_completed = true;
			return;
		}
	}
	// create a function and return it.
	Function* ret = new Function(rtype, param);
	// set function 
	this_namespace->set_new_func(name, ret);
	Mer::global_stmt() = false;
	_pcs.push_back(ret->pc);
	current_ins_table = &(ret->stmts);
	// off get the var_size of the function
	Parser::build_function_block();
	// count function size;
	ret->off = mem.function_block_size;
	mem.reset_func_block_size();

	_pcs.pop_back();
	ret->is_completed = true;
	Mer::global_stmt() = true;

}

bool Mer::Param::type_check(const std::vector<type_code_index>& types)
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

ParamFeature Mer::Param::get_param_feature()
{
	ParamFeature ret;
	for (auto a : arg_pos)
	{
		ret.push_back(a.first);
	}
	return ret;
}

Mer::FunctionBase::FunctionBase()
{
}

void Mer::FunctionBase::check_param(const std::vector<type_code_index>& types)
{
	if (types.size() != param_types.size())
	{
		throw Error("A03 argument size error expect " + std::to_string(param_types.size()) + " but receive " + std::to_string(types.size()));

	}
	for (size_t i = 0; i < param_types.size(); i++)
	{
		if (types[i] == param_types[i])
		{
			continue;
		}
		auto type_seeker = Mem::type_map.find(types[i]);
		if (type_seeker == Mem::type_map.end())
		{
			throw Error("A01 exists an undefined type : type code " + std::to_string(types[i]));
		}
		if (!type_seeker->second->convertible(param_types[i]))
		{
			throw Error("A02 Error: type not matched type: " + Mem::type_to_string(Mem::BasicType(param_types[i])) + " to " + Mem::type_to_string(Mem::BasicType(types[i])));
		}
	}
	return;
}

std::string Mer::FunctionBase::to_string(std::string name) const
{
	return	"function " + name + param_feature_to_string(param_types);

}
//================================================================

Mer::Function::Function(type_code_index t, Param* p) :
	type(t), param(p)
{
	for (const auto& a : param->get_param_table())
	{
		param_types.push_back(a.first);
	}
}

Mer::Function::Function(type_code_index t) :type(t) {}

Mem::Object Mer::Function::run(const std::vector<Mem::Object>& objs)
{
	mem.new_func(off);
	auto param_table = param->get_param_table();
	for (size_t i = 0; i < param->get_param_table().size(); i++)
	{
		mem[mem.get_current() + param_table[i].second] = objs[i];
	}
	size_t tmp = *pc;
	for (*pc = 0; *pc < stmts.size(); ++ * pc)
		stmts[*pc]->execute();
	*pc = tmp;
	mem.end_func();
	return function_ret;
}

Mer::Function::~Function()
{
	delete pc;
	delete param;
}

void Mer::SystemFunction::check_param(const std::vector<type_code_index>& types)
{
	if (check_param_type)
		FunctionBase::check_param(types);
}

Mer::SystemFunction::~SystemFunction()
{

}

bool Mer::InitKey::operator<(const InitKey& init_key) const
{
	if (type_code != init_key.type_code)
		return type_code < init_key.type_code;
	if (params.size() != init_key.params.size()) {
		return params.size() < init_key.params.size();
	}
	for (size_t i = 0; i < params.size(); i++)
	{
		if (params[i] != init_key.params[i])
			return params[i] < init_key.params[i];
	}
	return false;
}

bool Mer::compare_param_feature(const std::vector<type_code_index>& p1, const std::vector<type_code_index>& p2)
{
	if (p1.size() != p2.size()) {
		return p1.size() < p2.size();
	}
	auto sz = p1.size();
	for (std::vector<type_code_index>::size_type i = 0; i < sz; i++)
	{
		if (p1[i] != p2[i])
			return p1[i] < p2[i];
	}
	return false;
}

std::string Mer::param_feature_to_string(const ParamFeature& pf)
{
	if (pf.empty())
		return "()";
	std::string ret = "(";
	for (int i = 0; i < (int)pf.size() - 1; i++)
	{
		
		ret += type_to_string(pf[i]) + ',';
	}
	ret += type_to_string(pf.back()) + ')';
	return ret;
}
