#pragma once
#include "../../Lexer/HPP/Lexer.hpp"
#include "../../Runtime/HPP/Memory.hpp"
#include "Error.hpp"
namespace Simcc
{
	namespace Parser
	{
		// 储存编译时需要的生成的数据. 
		class Environment
		{
		public:
			static Simcc::Runtime::Stack stack_block;
			static TokenStream* token_stream;
			static size_t current_pos;
			//===========
			static Lexer::Token* this_token()
			{
				return token_stream->operator[](current_pos);
			}
			static void match(Lexer::Tag t)
			{
				if (this_token()->get_tag() == t)
				{
					current_pos++;
					return;
				}
				else if (this_token()->get_tag() == Lexer::Endl)
				{
					current_pos++;
					match(t);
				}
				else
					throw Error::SyntaxError(this_token()->to_string() + "unexpected erorr!");
			}
			static bool match_noexcept(Lexer::Tag t)
			{
				if (this_token()->get_tag() == t)
				{
					return true;
				}
				else if (this_token()->get_tag() == Lexer::Endl) {
					current_pos++;
					return match_noexcept(t);
				}
				return false;
			}
		};
	}
}