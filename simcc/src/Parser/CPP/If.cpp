#include "..\HPP\If.hpp"
#include "../HPP/Block.hpp"
using namespace Simcc::Parser;
If::If()
{
	Environment::match(Lexer::TIf);
	Environment::match(Lexer::Lk);
	// 由于表达式可能会识别if中的 )所以我们也将加入(使得表达式平衡.
	auto cond = new Expression();
	Environment::match(Lexer::Rk);
	block.push_back({ cond,new Block() });
	while (true)
	{
		if (Environment::match_noexcept(Lexer::TElif))
		{
			Environment::current_pos++;
			Environment::match(Lexer::Lk);
			Environment::current_pos--;
			auto cond = new Expression();
			Environment::current_pos--;
			Environment::match(Lexer::Rk);
			block.push_back({ cond,new Block() });
			continue;
		}
		else if (Environment::match_noexcept(Lexer::TElse))
		{
			std::cout << "SHIT2" << std::endl;
			Environment::current_pos++;
			Else = new Block();
			return;
		}
		return;
	}

}


void Simcc::Parser::If::execute()
{
	for (auto & a : block)
	{
		if (a.first->GetResult()->to_bool())
		{
			a.second->execute();
			return;
		}

	}
	if (Else != nullptr)
		Else->execute();
}

Simcc::Parser::If::~If() {
	for (auto &a : block)
	{
		delete a.first;
		delete a.second;
	}
	if (Else != nullptr)
		delete Else;
}
