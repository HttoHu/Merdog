#include "..\HPP\Action.hpp"
using namespace Simcc::Parser;
Simcc::Runtime::Action::Action()
{
	switch (Environment::this_token()->get_tag())
	{
	case Lexer::System:
		action_type = SYSTEM_CALL;
		Environment::current_pos++;
		Environment::match(Lexer::Place);
		content = Environment::this_token();
		Environment::match(Lexer::Id);
		param = new Parser::Param();
		return;
	case Lexer::PP:
		action_type = FP;
		Environment::current_pos++;
		content = Environment::this_token();
		Environment::current_pos++;
		return;
	case Lexer::MM:
		action_type = FM;
		Environment::current_pos++;
		content = Environment::this_token();
		return;
	case Lexer::Id:
		content = Environment::this_token();
		Environment::current_pos++;
		switch (Environment::this_token()->get_tag())
		{
		case Lexer::Lk:
			action_type = CALL;
			param = new Parser::Param();
			return;
		case Lexer::PP:
			action_type = BP;
			Environment::current_pos++;
			return;
		case Lexer::MM:
			action_type = BM;
			Environment::current_pos++;
			return;
		default:
			action_type = ID;
			return;
		}
	default:
		content = Environment::this_token();
		action_type = Action::CS;
		return;
	}
}