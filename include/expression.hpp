#pragma once
#include"lexer.hpp"
#include "environment.hpp"

namespace Merdog
{
	char get_priority(TokenType a);
	class Expr
	{
	public:
		Expr();
		void _show()
		{
			for (const auto &a : value_stack)
			{
				std::cout << a->to_string();
			}
		}
	private:
		std::vector<ActionBase*> value_stack;
		Block *block;
	};
}