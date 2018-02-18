#include "../include/lexer.hpp"
using namespace Mer;

std::map<Tag, std::string> Mer::TagStr{
	{VAR,"VAR"},{PROGRAM,"PROGRAME"},{COMMA,"COMMA"},{COLON,"COLON"},
	{ID,"ID"},{INTEGER,"INTEGER"},{ REAL,"REAL" } ,
	{INTEGER_DECL,"INTEGER_DECL"},{REAL_DECL,"REAL_DECL"}, {STRING_DECL,"STRING_DECL"},
	{PLUS,"PLUS"},{MINUS,"MINUS"},{MUL,"MUL"},{DIV,"DIV"},
	{LPAREN,"LPAREN"},{RPAREN,"RPAREN"},
	{DOT,"DOT"},{BEGIN,"BEGIN"},{END,"END"},
	{SEMI,"SEMI"},{ASSIGN,"ASSIGN"},
	{ENDL,"ENDL"},{PRINT,"PRINT"}
};
std::map<std::string, Token*> Mer::KeyWord{
	{"print",new Token(PRINT)},
	{"string",new Token(STRING_DECL)},
	{"var",new Token(VAR)},{"begin",new Token(BEGIN)},
	{"end",new Token(END)},{"real",new Token(REAL_DECL)},
	{"int",new Token(INTEGER_DECL)},{"program",new Token(PROGRAM)}
};
Token* Mer::END_TOKEN = new Token(ENDOF);
TokenStream Mer::token_stream;
Token * Mer::parse_number(const std::string &str, size_t &pos)
{
	int64_t ret = 0;
	for (; pos < str.size(); pos++)
	{
		if (isdigit(str[pos]))
			ret = ret * 10 + (str[pos] - 48);
		else 
		{
			break;
		}
	}
	double tmp=0.0;
	double tmp2 = 1;
	if (str[pos] == '.')
	{
		pos++;
		for (; pos < str.size(); pos++)
		{
			if (isdigit(str[pos]))
			{
				tmp2 /= 10;
				tmp = tmp + tmp2*(str[pos] - 48);
			}
			else
			{
				return new Real((double)ret+tmp);
			}
		}
	}
	return new Integer(ret);
}
Token *Mer::parse_word(const std::string &str, size_t &pos)
{
	std::string ret;
	bool first_char = true;
	for (; pos < str.size(); pos++)
	{
		if (first_char)
		{
			first_char = false;
			if (isalpha(str[pos]) || str[pos] == '_')
				ret += str[pos];
			else
				throw Error("a word must begin with alpha or '_' ");
			continue;
		}
		else
		{
			if (isalnum(str[pos]) || str[pos] == '_')
				ret += str[pos];
			else
			{
				pos--;
				break;
			}
		}
	}
	auto result = KeyWord.find(ret);
	if (result != KeyWord.end())
	{
		return result->second;
	}
	auto id_result=Id::find(ret);
	if (id_result != nullptr)
		return id_result;
	else
		return new Id(ret);
}
Token * Mer::parse_string(const std::string & str, size_t & pos)
{
	std::string value;
	if (str[pos] == '\'')
		pos++;
	else
		throw Error("a string-literal must start with\'");
	for (; pos < str.size(); pos++)
	{
		if (str[pos] == '\'')
			break;
		if (str[pos] == '\\')
		{
			pos++;
			if (pos >= str.size())
				throw Error("out of range");
			switch (str[pos])
			{
			case 'n':
				value += '\n';
				break;
			case 't':
				value += '\t';
				break;
			case '\\':
				value += '\\';
				break;
			case '\'':
				value += '\'';
				break;
			default:
				throw Error("no matched escape character");
			}
		}
		else
			value += str[pos];
	}
	return new String(value);
}
void Mer::build_token_stream(const std::string &content)
{
	std::string tmp_str;
	token_stream.push_back(new Endl());
	for (size_t i = 0; i < content.size(); i++)
	{
		switch (content[i])
		{
		case '\'':
			token_stream.push_back(parse_string(content,i));
			break;
		case '\n':
			token_stream.push_back(new Endl());
			break;
		case '{':
			while (true)
			{
				if (i + 1 >= content.size())
					throw Mer::Error("{} not matched");
				else if (content[++i] == '}')
					break;
			}
			break;
		case ',':
			token_stream.push_back(new Token(COMMA));
			break;
		case ':':
			if (i + 1 < content.size() && content[i + 1] == '=')
			{ 
				token_stream.push_back(new Token(ASSIGN));
				i++;
			}
			else
				token_stream.push_back(new Token(COLON));
			break;
		case ';':
			token_stream.push_back(new Token(SEMI));
			break;
		case '.':
			token_stream.push_back(new Token(DOT));
			break;
		case '\t':
			break;
		case ' ':
			break;
		case '(':
			token_stream.push_back(new Token(LPAREN));
			break;
		case ')':
			token_stream.push_back(new Token(RPAREN));
			break;
		case '*':
			token_stream.push_back(new Token(MUL));
			break;
		case '/':
			token_stream.push_back(new Token(DIV));
			break;
		case '+':
			token_stream.push_back(new Token(PLUS));
			break;
		case '-':
			token_stream.push_back(new Token(MINUS));
			break;
		case '0':case '1':case '2':case '3':case '4':case '5':case '6':
		case '7':case '8':case '9':
			token_stream.push_back(parse_number(content, i));
			i--;
			break;
		default:
			token_stream.push_back(parse_word(content, i));
			break;
		}
	}
}