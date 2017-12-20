#include "../HPP/Loop.hpp"
#include "../HPP/Block.hpp"
#include "../../Runtime/HPP/Action.hpp"
using namespace Simcc::Parser;

Simcc::Parser::While::While()
{
	Environment::match(Lexer::TWhile);
	Environment::match(Lexer::Lk);
	condition = new Expression();
	Environment::match(Lexer::Rk);
	block = new Block();
}

void Simcc::Parser::While::execute()
{
	auto a = condition->GetResult();
	while (a->to_bool())
	{
		try
		{
			block->execute();
			delete a;
			a = condition->GetResult();
		}
		catch (char ch)
		{
			switch (ch)
			{
			case 1:
				return;
			case 2:
				continue;
			default:
				break;
			}
		}

	}
}

Simcc::Parser::While::~While() {
	delete condition;
	delete block;
}

Simcc::Parser::For::For()
{
	Environment::match(Lexer::TFor);
	Environment::match(Lexer::Lk);
	if (Environment::match_noexcept(Lexer::EndStmt))
		cbt = nullptr;
	else
	{
		cbt = new CreateBasicTypeObject();
	}
	condition = new Expression();
	Environment::match(Lexer::EndStmt);
	if (Environment::match_noexcept(Lexer::EndStmt))
	{
		Environment::current_pos++;
		step_action = nullptr;
	}
	else
	{
		step_action = new Runtime::Action();
	}

	Environment::match(Lexer::Rk);
	block = new Block();

}

void Simcc::Parser::For::execute()
{
	if (cbt != nullptr)
		cbt->execute();
	if (step_action != nullptr)
	{
		auto a = condition->GetResult();
		while (a->to_bool())
		{
			block->execute();
			step_action->execute();
			delete a;
			a = condition->GetResult();
		}
	}
	else
	{
		auto a = condition->GetResult();
		while (a->to_bool())
		{
			block->execute();
			delete a;
			a = condition->GetResult();
		}
	}
}

Simcc::Parser::For::~For()
{
	if (step_action != nullptr)
		delete step_action;
	if (cbt != nullptr)
		delete cbt;
	delete condition;
}
