#include "../HPP/Expression.hpp"
#include "../../Runtime/HPP/Action.hpp"
using namespace Simcc::Parser;

Simcc::Parser::Expression::Expression()
{
	int bd = 0;
	std::vector<Lexer::Token* >operatorStack;
	while (1)
	{
		//检查括号深度
		switch (this_token()->get_tag())
		{
		case Lexer::True:
		case Lexer::False:
		case Lexer::Tag::TLiteralDouble:
		case Lexer::Tag::TLiteralString:
		case Lexer::Tag::TLiteralInt:
		case Lexer::Tag::TLiteralLong:
		case Lexer::Tag::TLiteralChar:
			//  字面值总是先压入输入中.
			count_stack.push_back(new Runtime::Action());
			break;
		case Lexer::Tag::Lk:
			// '(' 总是被压入栈中
			bd++;
			operatorStack.push_back(this_token());
			break;
		case Lexer::Tag::Ne:
		case Lexer::Tag::Ge:
		case Lexer::Tag::Eq:
		case Lexer::Tag::Gt:
		case Lexer::Tag::Le:
		case Lexer::Tag::Lt:
		case Lexer::Tag::Or:
		case Lexer::Tag::Add:
		case Lexer::Tag::Sub:
		case Lexer::Tag::Mul:
		case Lexer::Tag::Div:
			//顶部遇到了比自己厉害的, 然后就pass-pass, 直到比自己差点的. 
			//当然一样厉害的还是pass 不敢打啊.'('除外啦. 毕竟它走他也不走 它只认')'
			while (!operatorStack.empty() && operatorStack.back()->get_tag() != Lexer::Tag::Lk&&get_priority(this_token()->get_tag()) <= get_priority(operatorStack.back()->get_tag()))
			{
				count_stack.push_back(new Runtime::Action(operatorStack.back()));
				operatorStack.pop_back();
			}
			operatorStack.push_back(this_token());
			break;
		case Lexer::Tag::Rk:
			bd--;
			if (bd < 0)
			{
				while (!operatorStack.empty())
				{
					count_stack.push_back(new Runtime::Action(operatorStack.back()));
					operatorStack.pop_back();
				}
				return;
			}
			while (operatorStack.back()->get_tag() != Lexer::Tag::Lk)
			{
				count_stack.push_back(new Runtime::Action(operatorStack.back()));
				operatorStack.pop_back();
			}
			operatorStack.pop_back();
			break;
		case Lexer::Tag::System:
		case Lexer::Tag::MM:
		case Lexer::Tag::PP:
		case Lexer::Tag::Id:
			count_stack.push_back(new Runtime::Action());
			continue;
		default:
			if (count_stack.empty())
				throw std::runtime_error("runtime_error21");
			while (!operatorStack.empty())
			{
				count_stack.push_back(new Runtime::Action(operatorStack.back()));
				operatorStack.pop_back();
			}
			return;
		}
		Environment::current_pos++;
	}

}


Simcc::Runtime::ObjectBase * Simcc::Parser::Expression::GetResult()
{
	using namespace Simcc::Lexer;
	std::deque<Runtime::ObjectBase*> tmpStack;
	for (size_t i = 0; i < count_stack.size(); i++)
	{
		switch (count_stack[i]->get_cs())
		{
		case TLiteralChar:
			tmpStack.push_front(new Runtime::ObjectBase(*(char*)count_stack[i]->content->get_value()));
			break;
		case TLiteralDouble:
			tmpStack.push_front(new Runtime::ObjectBase(*(double*)count_stack[i]->content->get_value()));
			break;
		case TLiteralInt:
			tmpStack.push_front(new Runtime::ObjectBase(*(int32_t*)count_stack[i]->content->get_value()));
			break;
		case TLiteralString:
			tmpStack.push_front(new Runtime::ObjectBase(*(std::string*)count_stack[i]->content->get_value()));
			break;
		case TLiteralLong:
			tmpStack.push_front(new Runtime::ObjectBase(*(int64_t*)count_stack[i]->content->get_value()));
			break;
		case True:
			tmpStack.push_front(new Runtime::ObjectBase(true));
			break;
		case False:
			tmpStack.push_front(new Runtime::ObjectBase(false));
			break;
		case Ne:
		case Ge:
		case Eq:
		case Gt:
		case Le:
		case And:
		case Or:
		case Lt:
		{
			Runtime::ObjectBase *newObj = (tmpStack[1])->Compare(tmpStack[0], count_stack[i]->content->get_tag());
			delete tmpStack[0];
			tmpStack.pop_front();
			delete tmpStack[0];
			tmpStack.pop_front();
			tmpStack.push_front(newObj);
			break;
		}
		case Add:
		{
			(tmpStack[1])->SelfAdd(tmpStack[0]);
			delete tmpStack[0];
			tmpStack.pop_front();
			break;
		}
		case Sub:
		{
			(tmpStack[1])->SelfSub(tmpStack[0]);
			delete tmpStack[0];
			tmpStack.pop_front();
			break;
		}
		case Mul:
		{
			(tmpStack[1])->SelfMul(tmpStack[0]);
			delete tmpStack[0];
			tmpStack.pop_front();
			break;
		}
		case Div:
		{
			(tmpStack[1])->SelfDiv(tmpStack[0]);
			delete tmpStack[0];
			tmpStack.pop_front();
			break;
		}
			break;
		case Simcc::Lexer::Tag::System:
		case MM:
		case PP:
		case Id:
			tmpStack.push_front(count_stack[i]->get_object_result());
			break;
		default:
			break;
		}
	}
	if (tmpStack.empty())
		return nullptr;
	return tmpStack.front();
}
void Simcc::Parser::Expression::debug()
{
	for (auto & a : count_stack)
	{
		std::cout << a->content->to_string();
	}
}

Simcc::Parser::Expression::~Expression()
{
	for (auto &a : count_stack)
		delete a;
}
