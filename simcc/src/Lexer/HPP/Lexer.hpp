#pragma once
#include "Id.hpp"
#include "Literal.hpp"
#include "EndLine.hpp"
#include <string>
#include <deque>
typedef std::vector<Simcc::Lexer::Token*> TokenStream;
namespace Simcc
{
	namespace Lexer
	{
		class Lexer
		{
		public:
			Lexer(const std::string & str):content(str){
				token_stream.push_back(new EndLine);
			}
			void read_string();
			void read_char();
			void read_number();
			void read_word();
			void read_symbol();
			void debug();
			void init_token_stream();
			TokenStream token_stream;
			~Lexer()
			{
			}
		private:
			std::string content;
			size_t index=0;
		};
	}
}
