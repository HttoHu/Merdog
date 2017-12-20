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
		//�ڼ���򵥵����ʱ��++i; �����ĵ���, ֻ�е����¼�����ʱ����һ�����ʽ�Ƿǳ��������, ��ActionStmt����������������
		//����for���Ա���ǰ��ܶ�.
		class ActionStmt :public Stmt
		{
		public:
			// һ�������, ������Ϊ++i;�����������Single���, Single�����жϱ��ʽ�Ƿ�ֻ��һ���¼�, ���Թ���һ��ActionStmtһ����˵���ǲ���
			// Ĭ�Ϲ��캯��...
			ActionStmt();
			//ע��, Singleִ������ɾ�����ʽ, ͬʱ���ʽ�����ActionҲ�ᱻɾ��.. ����������Ҫ���¿���һ��... 
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