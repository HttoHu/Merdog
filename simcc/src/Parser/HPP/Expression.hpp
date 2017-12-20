#pragma once
#include "Environment.hpp"
#include "../../Runtime/HPP/Object.hpp"
#include "../../Lexer/HPP/Literal.hpp"
#include <stack>
#include <memory>
/*
* ˼· : �﷨������ɶ��ûд��.
* �������ǹ���һ��Expression ��,
* �������һ������, Ȼ�����ֱ�Ӽ�������ʽ��ֵ, ��Ȼ����idʱ���ǵ���һ��
* �� get_value����, ����һ��Object Ȼ��
*/
namespace Simcc
{
	namespace Runtime
	{
		class Action;
	}
	namespace Parser
	{
		class Expression
		{
		public:
			Expression();
			// ���ص�ֵ��Ҫdelete
			Runtime::ObjectBase* GetResult();
			void debug();
			size_t action_size()const
			{
				return count_stack.size();
			}
			Runtime::Action* get_front_action()
			{
				return count_stack.front();
			}
			~Expression();
		private:
			Lexer::Token * next_token()
			{
				return Environment::token_stream->operator[](Environment::current_pos++);
			}
			Lexer::Token * this_token()
			{
				return Environment::token_stream->operator[](Environment::current_pos);
			}
			bool match(Lexer::Tag t)
			{
				if ((Environment::token_stream)->operator[](Environment::current_pos++)->get_tag() == t)
				{
					return true;
				}
				return false;
			}
			char get_priority(Simcc::Lexer::Tag a)
			{
				switch (a)
				{
				case Simcc::Lexer::And:
				case Simcc::Lexer::Or:
					return 0;
				case Simcc::Lexer::Ge:
				case Simcc::Lexer::Gt:
				case Simcc::Lexer::Le:
				case Simcc::Lexer::Lt:
				case Simcc::Lexer::Ne:
				case Simcc::Lexer::Eq:
					return 1;
				case Simcc::Lexer::Add:
				case Simcc::Lexer::Sub:
					return 2;
				case Simcc::Lexer::Mul:
				case Simcc::Lexer::Div:
					return 3;
				case Simcc::Lexer::Lk:
					return 10;
				default:
					throw std::runtime_error("runtime_error4");
					break;
				}
			}
			std::vector<Runtime::Action*> count_stack;
		};
	}
}