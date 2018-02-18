#pragma once
#include "Expression.hpp"
#include "Stmt.hpp"
namespace Simcc
{
	namespace Parser
	{
		class CreateBasicTypeObject :public Stmt
		{
		public:
			CreateBasicTypeObject()
			{
				type = next_token();
				while (true)
				{
					Lexer::Token *tok = next_token();
					Lexer::Tag tag = next_token()->get_tag();
					Expression *expr = new Expression();
					
					vtable.push_back({ tok,expr });
					if (tag != Lexer::Assign)
					{
						throw Error::SyntaxError("In (_CreateBasicTypeObject)"+this_token()->to_string()+ "unexpected character.");
					}
					tag = next_token()->get_tag();
					if (tag == Lexer::Comma)
						continue;
					else if (tag == Lexer::EndStmt)
						return;
					else
					{ 
						throw Error::SyntaxError("In (_CreateBasicTypeObject)"+this_token()->to_string()+ "unexpected character.");
					}
				}
			}
			void execute()override;
			Lexer::Token* next_token();
			Lexer::Token* this_token();
		private:
			Lexer::Token* type;
			std::list < std::pair<Lexer::Token*, Expression*>> vtable;
		};
	}
}