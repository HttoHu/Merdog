#pragma once
#include "Environment.hpp"
#include "Expression.hpp"
#include "CreateObject.hpp"
#include "Single.hpp"
#include "Loop.hpp"
namespace Simcc
{
	namespace Parser
	{
		class Block
		{
		public:
			class Return :public Stmt
			{
			public:
				Return() {
					Parser::Environment::match(Lexer::TReturn);
					if (Parser::Environment::match_noexcept(Lexer::EndStmt))
					{
						Parser::Environment::current_pos++;
						expr = nullptr;
					}
					expr = new Expression();
					Parser::Environment::match(Lexer::EndStmt);
				}
				void execute()override {
					throw expr->GetResult();
				}
			private:
				Expression *expr;
			};
			class Break :public Stmt
			{
			public:
				Break() {
					Parser::Environment::match(Lexer::TBreak);
					Parser::Environment::match(Lexer::EndStmt);
				}
				void execute()override {
					throw (char)1;
				}
			private:
			};
			class Continue :public Stmt
			{
			public:
				Continue() {
					Parser::Environment::match(Lexer::TContinue);
					Parser::Environment::match(Lexer::EndStmt);
				}
				void execute()override {
					throw (char)2;
				}
			private:
			};
			Block();
			void execute()
			{
				index = 0;
				for (const auto & a : stmts)
				{
					a->execute();
				}
			}
			void break_block()
			{
				index = stmts.size();
			}
			~Block() {
				for (auto &a : stmts)
					delete a;
			}
		private:
			std::deque<Stmt*> stmts;
			size_t index = 0;
		};
	}
}