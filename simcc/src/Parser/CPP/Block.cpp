#include "../HPP/Block.hpp"
#include "../../Runtime/HPP/Action.hpp"
#include "../../System/HPP/SystemFunction.hpp"
using namespace Simcc::Parser;
Block::Block()
{
	Parser::Environment::current_pos++;
	bool isSingle = !(Parser::Environment::this_token()->get_tag() == Lexer::Tag::BlockBegin);
	while (isSingle)
	{
		switch (Parser::Environment::this_token()->get_tag())
		{
		case Lexer::Tag::System:
			Parser::Environment::current_pos++;
			Environment::match(Lexer::Place);
			stmts.push_back(new System::CPPFunction());
			return;
		case Lexer::Tag::TReturn:
			stmts.push_back(new Return());
			Parser::Environment::current_pos++;
			return;
		case Lexer::Tag::TBreak:
			stmts.push_back(new Break());
			Parser::Environment::current_pos++;
			return;
		case Lexer::Tag::TContinue:
			stmts.push_back(new Continue());
			Parser::Environment::current_pos++;
			return;
		case Lexer::Tag::Endl:
			Parser::Environment::current_pos++;
			break;
		case Lexer::Tag::TWhile:
			stmts.push_back(new Simcc::Parser::While());
			Parser::Environment::current_pos++;
			return;
		case Lexer::Tag::TIf:
			stmts.push_back(new Simcc::Parser::If());
			Parser::Environment::current_pos++;
			return;
		case Lexer::Tag::TFor:
			stmts.push_back(new Simcc::Parser::For());
			Parser::Environment::current_pos++;
			return;
		case Lexer::Tag::SBool:
		case  Lexer::Tag::SInt:
		case  Lexer::Tag::SDouble:
		case  Lexer::Tag::SString:
		case  Lexer::Tag::SChar:
		case  Lexer::Tag::SLong:
			stmts.push_back(new CreateBasicTypeObject());
			Parser::Environment::current_pos++;
			return;
		case Lexer::Tag::MM:
		case Lexer::Tag::PP:
		case Lexer::Tag::Id:
			if (Parser::Environment::token_stream->at(Parser::Environment::current_pos + 1)->get_tag() == Lexer::Assign)
				stmts.push_back(new Assign());
			else
			{
				auto a = new Single();
				if (a->is_simple())
				{
					stmts.push_back(new ActionStmt(new Runtime::Action(*a->create_action_stmt())));
					delete a;
					return;
				}
				else
					stmts.push_back(a);
			}
			return;
		default:
			throw std::runtime_error("\n" + Parser::Environment::this_token()->to_string() + "runtime_error13");
		}
	}
	Parser::Environment::current_pos++;
	while (!isSingle)
	{
		if (Parser::Environment::current_pos >= Parser::Environment::token_stream->size())
		{
			std::cout << std::endl<<Parser::Environment::current_pos;
			throw std::runtime_error("runtime_error15");

		}
		switch (Parser::Environment::this_token()->get_tag())
		{
		case Lexer::Tag::System:
			Parser::Environment::current_pos++;
			Environment::match(Lexer::Place);
			stmts.push_back(new System::CPPFunction());
			break;
		case Lexer::Tag::TReturn:
			stmts.push_back(new Return());
			break;
		case Lexer::Tag::TBreak:
			stmts.push_back(new Break());
			break;
		case Lexer::Tag::TContinue:
			stmts.push_back(new Continue());
			break;
		case Lexer::Tag::BlockEnd:
			Parser::Environment::current_pos++;
			return;
		case Lexer::Tag::Endl:
			Parser::Environment::current_pos++;
			continue;
		case Lexer::Tag::TWhile:
			stmts.push_back(new Simcc::Parser::While());
			break;
		case Lexer::Tag::TIf:
			stmts.push_back(new Simcc::Parser::If());
			break;
		case Lexer::Tag::TFor:
			stmts.push_back(new Simcc::Parser::For());
			break;
		case Lexer::Tag::SBool:
		case  Lexer::Tag::SInt:
		case  Lexer::Tag::SDouble:
		case  Lexer::Tag::SString:
		case  Lexer::Tag::SChar:
		case  Lexer::Tag::SLong:
			stmts.push_back(new CreateBasicTypeObject());
			continue;
		case Lexer::Tag::MM:
		case Lexer::Tag::PP:
		case Lexer::Tag::Id:
			if (Parser::Environment::token_stream->at(Parser::Environment::current_pos + 1)->get_tag() == Lexer::Assign)
				stmts.push_back(new Assign());
			else
			{
				auto a = new Single();
				if (a->is_simple())
				{
					stmts.push_back(new ActionStmt(new Runtime::Action(*a->create_action_stmt())));
					delete a;
					continue;
				}
				else
					stmts.push_back(a);
			}
			continue;
		default:
			throw Error::SyntaxError( "In (_Block::Block)"+Parser::Environment::this_token()->to_string() + "unexpeted lexem");
		}
	}
}