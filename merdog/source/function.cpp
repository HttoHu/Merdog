#include "../include/function.hpp"
#include "../include/var.hpp"
#include "../include/memory.hpp"
#include "../include/parser.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
#define MER2_1_1
using namespace Mer;
std::map<std::string, Function*> Mer::function_table;
Block *Mer::current_function_block = nullptr;
size_t Mer::this_func_type;
//=============================================================
bool		is_function_statement()
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
		if (token_stream.this_token(index + 1)->get_tag() == SEMI)
		{
			for (int i = 0; i < index + 1; i++)
			{
				token_stream.next();
			}
			token_stream.match(SEMI);
			return true;
		}
		else
			return false;
	}
	else
		throw Error("it isn't a function");
}

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
		symbol_table->push(name, new VarIdRecorder(type, pos));
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
	this_namespace->sl_table->new_block();
	auto name = Id::get_value(token_stream.this_token());
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
		stack_memory.new_block();
		temp->param = build_param();
		// we use pure_block because we should push the param to the block, 
		// so we need to create a preserved memory for param.
		Block *blo = pure_block();
		temp->reset_block(blo);
		temp->is_completed = true;
		symbol_table->end_block();
		return;
	}
	this_namespace->sl_table->push_glo(name, new FuncIdRecorder(rtype));
	if (is_function_statement())
	{
		Function*ret = new Function(rtype);
		this_namespace->functions.insert({ name,ret });
		ret->is_completed = false;
		return;
	}
	// create a function and return it.
	stack_memory.new_block();
	Param *param = build_param();
	Function *ret = new Function(rtype, param, nullptr);
	this_namespace->functions.insert({ name,ret });
	Block *blo = pure_block();
	ret->reset_block(blo);
	ret->is_completed = true;
	symbol_table->end_block();

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
			std::cout << type_seeker->second->to_string() << std::endl;
			std::cout << arg_pos[i].second;
			return false;
		}
	}
	return true;
}
Mer::FunctionBase::FunctionBase()
{
}
bool Mer::FunctionBase::check_param(const std::vector<size_t>& types)
{
	if (types.size() != param_types.size())
	{
		return false;
	}
	for (size_t i = 0; i < param_types.size(); i++)
	{
		auto type_seeker = Mem::type_map.find(types[i]);
		if (type_seeker == Mem::type_map.end())
		{
			throw Error("A01 exists an undefined type");
		}
		if (!type_seeker->second->convertible(param_types[i]))
			return false;
	}
	return true;
}
//================================================================
void Mer::FunctionBase::set_index(size_t pos)
{
	index = pos;
}

Mer::Function::Function(size_t t, Param * p, Block * bl) :type(t), param(p), blo(bl)
{
	for (const auto &a : param->get_param_table())
	{
		param_types.push_back(a.first);
	}
}

Mer::Function::Function(size_t t, Block * bl) :type(t), blo(bl) {}
void Mer::Function::reset_block(Block * b) {
	blo = b;
}

void Mer::Function::reser_param(Param * p)
{
	for (const auto &a : param->get_param_table())
	{
		param_types.push_back(a.first);
	}
}

Mem::Object Mer::Function::run(std::vector<Mem::Object>& objs)
{
	stack_memory.new_func(index);
	auto param_table = param->get_param_table();
	for (size_t i = 0; i < param->get_param_table().size(); i++)
	{
		stack_memory[param_table[i].second] = objs[i];
	}
	auto ret = blo->execute();
	stack_memory.end_func();
	return ret;
}

bool Mer::SystemFunction::check_param(const std::vector<size_t>& types)
{
	if (check_param_type)
		return FunctionBase::check_param(types);
	return true;
}