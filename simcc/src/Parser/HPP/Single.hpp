#pragma once
#include "Stmt.hpp"
#include "Environment.hpp"
#include "Expression.hpp"
namespace Simcc
{
	namespace Runtime
	{
		class Action;
	}
	namespace Parser
	{
		//在计算简单的语句时如++i; 函数的调用, 只有单个事件运行时构建一个表达式是非常不划算的, 用ActionStmt可以显著提升性能
		//这样for可以比以前快很多.
		class ActionStmt :public Stmt
		{
		public:
			// 一般情况下, 我们认为++i;这样的语句是Single语句, Single可以判断表达式是否只有一个事件, 可以构造一个ActionStmt一般来说我们不用
			// 默认构造函数...
			ActionStmt();
			//注意, Single执行完后会删除表达式, 同时表达式里面的Action也会被删除.. 所以我们需要重新拷贝一个... 
			ActionStmt(Simcc::Runtime::Action *anc):action(anc)
			{
			}
			void execute()override;
		private:
			Simcc::Runtime::Action *action;
		};
		class Assign :public Stmt
		{
		public:
			Assign() 
			{
				id_name = Environment::this_token();
				Environment::current_pos++;
				Environment::match(Lexer::Assign);
				expr = new Expression();
				Environment::match(Lexer::EndStmt);
			}
			void execute()override
			{
				auto result = expr->GetResult();
				*Environment::stack_block.find_variable(id_name) = *result;
				delete result;
			}
			~Assign() { delete expr; }
		private:
			Expression *expr;
			Lexer::Token *id_name;
		};
		class Single :public Stmt
		{
		public:
			Single()
			{
				expr = new Expression();
				match(Lexer::EndStmt);
			}
			void execute()override
			{
				delete expr->GetResult();
			}
			bool is_simple()const {
				return expr->action_size() == 1;
			}
			Runtime::Action* create_action_stmt()
			{
				return expr->get_front_action();
			}
			Lexer::Token* next_token();
			bool match(Lexer::Tag t);
			~Single()
			{
				delete expr;
			}
		private:
			Expression *expr;
		};
	}
}