#include "../include/expression.hpp"
using namespace Merdog;

char Merdog::get_priority(TokenType a)
{
	switch (a)
	{
	case Merdog::And:
	case Merdog::Or:
		return 0;
	case Merdog::Ge:
	case Merdog::Gt:
	case Merdog::Le:
	case Merdog::Lt:
	case Merdog::Ne:
	case Merdog::Eq:
		return 1;
	case Merdog::Add:
	case Merdog::Sub:
		return 2;
	case Merdog::Mul:
	case Merdog::Div:
		return 5;
	case Merdog::LBk:
		return 10;
	default:
		throw Error("operator is not supported");
		break;
	}
}

Merdog::Expr::Expr():block(Environment::current_block())
{
	int bd = 0;
	std::vector<Token* >operatorStack;
	while (1)
	{
		switch (Environment::current_token()->get_token_type())
		{
		case Merdog::Line:
			Environment::move_to_next();
			continue;
		case Merdog::True:
		case Merdog::False:
		case Merdog::TLiteral:
			value_stack.push_back(new ActionLiteral(Environment::current_token()));
			break;
		case Merdog::LBk:
			bd++;
			operatorStack.push_back(Environment::current_token());
			break;
		case Merdog::Ne:
		case Merdog::Ge:
		case Merdog::Eq:
		case Merdog::Gt:
		case Merdog::Le:
		case Merdog::Lt:
		case Merdog::Or:
		case Merdog::Add:
		case Merdog::Sub:
		case Merdog::Mul:
		case Merdog::Div:
			while (!operatorStack.empty() && operatorStack.back()->get_token_type() != Merdog::LBk&&get_priority(Environment::current_token()->get_token_type()) 
				<= get_priority(operatorStack.back()->get_token_type()))
			{
				value_stack.push_back(new ActionBase(operatorStack.back()));
				operatorStack.pop_back();
			}
			operatorStack.push_back(Environment::current_token());
			break;
		case Merdog::RBk:
			bd--;
			if (bd < 0)
			{
				while (!operatorStack.empty())
				{
					value_stack.push_back(new ActionBase(operatorStack.back()));
					operatorStack.pop_back();
				}
				return;
			}
			while (operatorStack.back()->get_token_type() != Merdog::LBk)
			{
				value_stack.push_back(new ActionBase(operatorStack.back()));
				operatorStack.pop_back();
			}
			operatorStack.pop_back();
			break;
		case Merdog::MM:
		case Merdog::PP:
		case Merdog::TId:
			value_stack.push_back(new ActionBase(Environment::current_token()));
			Environment::move_to_next();
			continue;
		default:
			if (value_stack.empty())
				throw Error("bad expr");
			while (!operatorStack.empty())
			{
				value_stack.push_back(new ActionBase(operatorStack.back()));
				operatorStack.pop_back();
			}
			return;
		}
		Environment::move_to_next();
	}
}
