#pragma once
#include <iostream>
#include <fstream>
#include "lexer.hpp"
#include "block.hpp"
#define Debug
namespace Merdog
{
	struct Environment
	{
		static std::string &file_content()
		{
			static std::string ret;
			return ret;
		}
		static TokenStream &token_stream()
		{
			static TokenStream to;
			return to;
		}
		static size_t &token_index()
		{
			static size_t index = 0;
			return index;
		}
		static Token* current_token()
		{
			return token_stream()[token_index()];
		}
		static void	move_to_next()
		{
			token_index()++;
		}
		static void match(TokenType t)
		{
			if (current_token()->get_token_type() == t)
			{
				token_index()++;
				return;
			}
			else if (current_token()->get_token_type() == Line)
			{
				token_index()++;
				match(t);
			}
			else
				throw Error(current_token()->to_string() + "unexpected erorr!");
		}
		static bool match_noexcept(TokenType t)
		{
			if (current_token()->get_token_type() == t)
			{
				return true;
			}
			else if (current_token()->get_token_type() == Line) {
				token_index()++;
				return match_noexcept(t);
			}
			return false;
		}

		static Block *&glo_block()
		{
			static Block *block = new Block("gl",nullptr);
			return block;
		}
		static Block *&current_block()
		{
			static Block *block=glo_block();
			return block;
		}
		static Block *new_block()
		{
			auto blok=current_block()->create_block();
			current_block() = blok;
			return blok;
		}
		static void set_ts(const std::string &filename)
		{
			using namespace std;
			ifstream ifs(filename);
			if (!ifs)
				throw Error("error 02 :invailed file name "+filename+"\n");
			std::string content((istreambuf_iterator<char>(ifs)),istreambuf_iterator<char>());
			file_content() = content;
			scan();
		}
#ifdef Debug
		static void show_ts()
		{
			for (const auto &a : token_stream())
				std::cout << a->to_string();

		}
#else
#error PLEASE DELETE ME
#endif
	};
}