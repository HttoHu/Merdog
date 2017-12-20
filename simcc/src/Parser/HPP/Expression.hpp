#pragma once
#include "Environment.hpp"
#include "../../Runtime/HPP/Object.hpp"
#include "../../Lexer/HPP/Literal.hpp"
#include <stack>
#include <memory>
/*
* 思路 : 语法分析我啥都没写哩.
* 首先我们构造一个Expression 类,
* 该类接受一个环境, 然后可以直接计算出表达式的值, 当然遇到id时我们调用一个
* 叫 get_value的类, 返回一个Object 然后
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
			// 返回的值不要delete
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