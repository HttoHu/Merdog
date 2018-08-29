#include "../include/function.hpp"
#include "../include/var.hpp"
#include "../include/memory.hpp"
#include "../include/parser.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
#define MER2_1_1
using namespace Mer; 
Function *Mer::current_function=nullptr;
std::map<std::string, Function*> Mer::function_table;

size_t Mer::					this_func_type;

bool							is_function_statement()
{
	int index = 1;
	if (token_stream.this_tag() == LPAREN)
	{
		while (token_stream.this_token(index)->get_tag()!=RPAREN)
		{
			if (token_stream.this_token(index) == END_TOKEN)
				throw Error("Reached end of the file");
			index++;
		}
		if (token_stream.this_token(index + 1)->get_tag() == SEMI)
		{
			for (int i = 0; i < index+1; i++)
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

Param * Mer::Parser::			build_param()
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

void Mer::Parser::				build_function()
{
	token_stream.match(FUNCTION);
	size_t rtype = Mem::get_type_code(token_stream.this_token());
	this_func_type = rtype;
	token_stream.next();
	this_namespace->sl_table->new_block();
	auto name = Id::get_value(token_stream.this_token());
	token_stream.next();
#ifdef MER2_1_1
	// if the function has decleared.
	auto finder = this_namespace->functions.find(name);
	if (finder != this_namespace->functions.end())
	{
		if (finder->second->is_completed == true)
		{
			throw Error("function redefined.");
		}
		Function *temp = static_cast<Function*>(finder->second);
		current_function = temp;
		stack_memory.new_block();
		temp->param = build_param();

		Block *blo = pure_block();
		temp->reset_block(blo);
		temp->is_completed = true;
		this_namespace->sl_table->end_block();
		current_function = nullptr;
		return;
	}
#endif
	this_namespace->sl_table->push_glo(name, new FuncIdRecorder(rtype));
	if (is_function_statement())
	{
		Function*ret = new Function(rtype, nullptr);
		this_namespace->functions.insert({ name,ret });
		ret->is_completed = false;
		return;
	}
	stack_memory.new_block();
	Param *param = build_param();
	Function*ret = new Function(rtype, nullptr);
	current_function = ret;
	this_namespace->functions.insert({ name,ret });
	Block *blo = pure_block();
	ret->param = param;
	ret->reset_block(blo);
	ret->is_completed = true;
	this_namespace->sl_table->end_block();
	current_function = nullptr;
}

bool Mer::Param::				type_check(const std::vector<size_t>& types)
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

Mem::Object Mer::Function::		run(std::vector<Mem::Object>& objs)
{
	size_t reserved_size = param->get_param_table().size();
	stack_memory.new_func(reserved_size);
	for (auto *a : pos_info)
	{
		*a -= reserved_size;
		std::cout << "Current add:"<<a<<std::endl;
	}
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
	for (auto &a : pos_info)
	{
		*a += reserved_size;
	}
	return nullptr;
}
