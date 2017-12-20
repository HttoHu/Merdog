#pragma once
#include <unordered_map>
namespace Simcc
{
	namespace Lexer
	{
		class Token;
	}
	namespace Runtime
	{
		class IdTypeTable
		{
		public:
			static std::unordered_map<Lexer::Token*, size_t> type_table;
			static size_t find_type(Lexer::Token* tok);
		private:
		};
	}
}