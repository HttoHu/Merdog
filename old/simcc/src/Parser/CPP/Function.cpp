#include "../HPP/Function.hpp"
using namespace Simcc::Parser;
using namespace Simcc;
std::map<Lexer::Token*, Function*> Function::function_table;
Simcc::Parser::Param::Param()
{
	Parser::Environment::match(Lexer::Lk);
	while (1)
	{
		switch (Parser::Environment::this_token()->get_tag())
		{
		case Lexer::Id:
		case Lexer::TLiteralChar:
		case Lexer::TLiteralDouble:
		case Lexer::TLiteralLong:
		case Lexer::TLiteralInt:
		case Lexer::TLiteralString:
		case Lexer::True:
		case Lexer::False:
			param_list.push_back(Parser::Environment::this_token());
			break;
		default:
			Parser::Environment::match(Lexer::Rk);
			return;
		}
		Parser::Environment::current_pos++;
		if (Parser::Environment::match_noexcept(Lexer::Comma))
		{
			Parser::Environment::current_pos++;
			continue;
		}
		else
		{
			break;
		}
	}
	Parser::Environment::match(Lexer::Rk);
}
std::vector<Simcc::Runtime::ObjectBase*>& Simcc::Parser::Param::get_list()
{
	obj_list.clear();
	for (auto & a : param_list)
	{
		switch (a->get_tag())
		{
		case Lexer::Id:
			obj_list.push_back(new Simcc::Runtime::ObjectBase(*Parser::Environment::stack_block.find_variable(a)));
			break;
		case Lexer::TLiteralInt:
			obj_list.push_back(new Simcc::Runtime::ObjectBase(*(int*)a->get_value()));
			break;
		case Lexer::TLiteralChar:
			obj_list.push_back(new Simcc::Runtime::ObjectBase(*(char*)a->get_value()));
			break;
		case Lexer::TLiteralLong:
			obj_list.push_back(new Simcc::Runtime::ObjectBase(*(int64_t*)a->get_value()));
			break;
		case Lexer::TLiteralDouble:
			obj_list.push_back(new Simcc::Runtime::ObjectBase(*(double*)a->get_value()));
			break;
		case Lexer::TLiteralString:
			obj_list.push_back(new Simcc::Runtime::ObjectBase(*(std::string*)a->get_value()));
			break;
		default:
			throw std::runtime_error("RuntimeError");
			break;
		}
	}
	return obj_list;
}

Simcc::Parser::Param::~Param()
{
	for (auto & a : obj_list)
		delete a;
}

Simcc::Parser::Argument::Argument()
{
	Parser::Environment::match(Lexer::Lk);
	
	while (1)
	{
		std::pair<Lexer::Token*, Lexer::Token*> tmp;
		// 处理类型
		switch (Parser::Environment::this_token()->get_tag())
		{
		case Lexer::SInt:
		case Lexer::SDouble:
		case Lexer::SLong:
		case Lexer::SChar:
		case Lexer::SString:
		case Lexer::SBool:
			tmp.first = Parser::Environment::this_token();
			break;
		default:
			Parser::Environment::match(Lexer::Rk);
			return;
		}
		Parser::Environment::current_pos++;
		//id_name;
		tmp.second = Parser::Environment::this_token();
		Environment::match(Lexer::Id);
		arg_list.push_back(tmp);
		if (Parser::Environment::match_noexcept(Lexer::Comma))
		{
			Parser::Environment::current_pos++;
			continue;
		}
		break;
	}
	Parser::Environment::match(Lexer::Rk);
}

void Simcc::Parser::Argument::CreateVariable(Param & param)
{
	auto p_list = param.get_list();
	if (arg_list.size() != p_list.size())
		throw std::runtime_error("RUntimeError22");
	for (size_t i = 0; i<p_list.size(); i++)
	{
		Parser::Environment::stack_block.push(arg_list[i].second, p_list[i]);
	}
}

Simcc::Parser::Function::Function() {
	if (Parser::Environment::match_noexcept(Lexer::Id))
	{
		function_table.insert({ Parser::Environment::this_token(),this });
		Parser::Environment::current_pos++;
	}
	else
	{
		throw std::runtime_error("no matched");
	}
	args = new Argument();
	funcBlock = new Block();
}
Runtime::ObjectBase * Simcc::Parser::Function::execute(Param * param)
{
	Parser::Environment::stack_block.newBlock();
	args->CreateVariable(*param);
	try
	{
		funcBlock->execute();
	}
	catch (Runtime::ObjectBase* return_value)
	{
		Parser::Environment::stack_block.endBlock();
		return return_value;
	}
	Parser::Environment::stack_block.endBlock();
	return nullptr;
}

Runtime::ObjectBase * Simcc::Parser::Function::execute()
{
	Parser::Environment::stack_block.newBlock();
	try
	{
		funcBlock->execute();
	}
	catch (Runtime::ObjectBase* return_value)
	{
		Parser::Environment::stack_block.endBlock();
		return return_value;
	}
	Parser::Environment::stack_block.endBlock();
	return nullptr;
}
