#include "../include/lexer.hpp"
using namespace Mer;

std::map<Tag, std::string> Mer::TagStr{
	{REF,"REF"},{PROGRAM,"PROGRAME"},{COMMA,"COMMA"},{COLON,"COLON"},
	{ID,"ID"},{INTEGER,"INTEGER"},{ REAL,"REAL" } ,{FUNCTION,"FUNCTION"},{RETURN,"RETURN"},
	{IF,"IF"},{ELSE_IF,"ELSE_IF"},{ELSE,"ELSE"},{WHILE,"WHILE"},{FOR,"FOR"},{BREAK,"BREAK"},{CONTINUE,"CONTINUE"},
	{INTEGER_DECL,"INTEGER_DECL"},{REAL_DECL,"REAL_DECL"}, {STRING_DECL,"STRING_DECL"},{BOOL_DECL,"BOOL_DECL"},
	{PLUS,"PLUS"},{MINUS,"MINUS"},{MUL,"MUL"},{DIV,"DIV"},
	{GE,"GE"},{GT,"GT"},{LE,"LE"},{LT,"LT"},{EQ,"EQ"},{NE,"NE"},
	{AND,"AND"},{OR,"OR"},{NOT,"NOT"},{GET_ADD,"GET_ADD"},
	{LPAREN,"LPAREN"},{RPAREN,"RPAREN"},
	{DOT,"DOT"},{BEGIN,"BEGIN"},{END,"END"},
	{SEMI,"SEMI"},{ASSIGN,"ASSIGN"},
	{ENDL,"ENDL"},{PRINT,"PRINT"},
	{TRUE,"TRUE"}, {FALSE,"FALSE"},
};
std::map<std::string, Token*> Mer::KeyWord{
	{"if",new Token(IF)},{"elif",new Token(ELSE_IF)},{"else",new Token(ELSE)},
	{"while",new Token(WHILE)},{"break",new Token(BREAK)},{"for",new Token(FOR)},
	{"continue",new Token(CONTINUE)},
	{"function",new Token(FUNCTION)},{"return",new Token(RETURN)},
	{"print",new Token(PRINT)},{"true",new Token(TRUE)},
	{"false",new Token(FALSE)},
	{"string",new Token(STRING_DECL)},{"bool",new Token(BOOL_DECL)},
	{"ref",new Token(REF)},{"begin",new Token(BEGIN)},
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
	double tmp = 0.0;
	double tmp2 = 1;
	if (str[pos] == '.')
	{
		pos++;
		for (; pos < str.size(); pos++)
		{
			if (isdigit(str[pos]))
			{
				tmp2 /= 10;
				tmp = tmp + tmp2 * (str[pos] - 48);
			}
			else
			{
				return new Real((double)ret + tmp);
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
	auto id_result = Id::find(ret);
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
		case '\\':
			if (i + 1 < content.size() && content[i + 1] == '\\')
			{
				while (i + 1 < content.size() && content[++i] != '\n')
					continue;
			}
			else if (i + 1 < content.size() && content[i + 1] == '*')
			{
				i += 2;
				while (i < content.size())
				{
					i++;
					if (content[i] == '*')
					{
						i++;
						if (i < content.size() && content[i] == '\\')
							goto end;
					}
				}
			}
		end:break;
		case '\'':
			token_stream.push_back(parse_string(content, i));
			break;
		case '\n':
			token_stream.push_back(new Endl());
			break;
		case '{':
			Id::id_table().push_front(std::map<std::string, Id*>());
			token_stream.push_back(new Token(BEGIN));
			break;
		case '}':
			Id::id_table().pop_front();
			token_stream.push_back(new Token(END));
			break;
		case ',':
			token_stream.push_back(new Token(COMMA));
			break;
		case '&':
			if (i + 1 < content.size() && content[i + 1] == '&')
			{
				token_stream.push_back(new Token(AND));
				i++;
			}
			else
				token_stream.push_back(new Token(GET_ADD));
			break;
		case '|':
			if (i + 1 < content.size() && content[i + 1] == '|')
			{
				token_stream.push_back(new Token(OR));
				i++;
			}
			else
				throw Error("invalid token '|'");
			break;
		case '<':
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(new Token(LE));
				i++;
			}
			else
				token_stream.push_back(new Token(LT));
			break;
		case '>':
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(new Token(GE));
				i++;
			}
			else
				token_stream.push_back(new Token(GT));
			break;
		case '=':
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(new Token(EQ));
				i++;
				break;
			}
			else
				throw Error("invalid token '='");
		case '!':
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(new Token(NE));
				i++;
			}
			else
				token_stream.push_back(new Token(NOT));
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
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(new Token(SMUL));
				i++;
			}
			token_stream.push_back(new Token(MUL));
			break;
		case '/':
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(new Token(SDIV));
				i++;
			}
			token_stream.push_back(new Token(DIV));
			break;
		case '+':
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(new Token(SPLUS));
				i++;
			}
			token_stream.push_back(new Token(PLUS));
			break;
		case '-':
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(new Token(SMINUS));
				i++;
			}
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
	token_stream.push_back(END_TOKEN);
}