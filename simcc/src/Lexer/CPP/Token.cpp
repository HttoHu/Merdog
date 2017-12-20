#include "../HPP/Token.hpp"
using namespace Simcc::Lexer;
std::unordered_map<std::string, Tag>& Simcc::Lexer::keyword_map()
{
	static std::unordered_map<std::string, Tag> ret{
		{ "class",Tag::TClass },{ "namespace",Tag::TNamespace },{ "public",Tag::TPublic },{ "private",Tag::TPrivate },
		{ "true",Tag::True },{ "false",Tag::False },{"int",Tag::SInt},{"double",Tag::SDouble},{"long",Tag::SLong},
		{"char",Tag::SChar},{"string",Tag::SString},{"bool",Tag::SBool},
		{ "while",Tag::TWhile },{ "for",Tag::TFor },{ "continue",Tag::TContinue },{ "break",Tag::TBreak },
		{ "case",Tag::TCase },{ "break",Tag::TBreak },{ "goto",Tag::TBreak },
		{ "switch",Tag::TSwitch },{ "if",Tag::TIf },{ "else",Tag::TElse } ,{ "elif",Tag::TElif },
		{"return",Tag::TReturn},{"system",Tag::System}
	};
	return ret;
}

std::unordered_map<std::string, Tag>& Simcc::Lexer::symbol_map()
{
	static std::unordered_map<std::string, Tag> ret{
		{ "+",Tag::Add },{ "-",Tag::Sub },{ "*",Tag::Mul },{ "/",Tag::Div },
		{ "&&",Tag::And },{ "||",Tag::Or },{ "[",Tag::LSB },{ "]",Tag::RSB },
		{ "(",Tag::Lk },{ ")",Tag::Rk },
		{ "::",Tag::Place },{ "=",Tag::Assign },{ ">",Tag::Gt },{ "<",Tag::Lt },
		{ ">=",Tag::Ge },{ "<=",Tag::Le },{ "==",Tag::Eq },{ "!=",Tag::Ne },
		{",",Tag::Comma},{";",Tag::EndStmt},{".",Tag::MemberPoint},
		{":",Tag::TTag},{"++",Tag::PP},{"--",Tag::MM},{"#",Tag::Function}
	};
	return ret;
}