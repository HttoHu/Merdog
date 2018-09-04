/*
* Inspired by
* https://ruslanspivak.com/lsbasi-part10/
* Ruslan's Blog
* C++ Version.
* Yuantao Hu 2018
* Email Huyuantao@outlook.com
*/
#include "../include/lexer.hpp"
using namespace Mer;
using TokenMap = std::map<std::string, Token*>;
using TagStrMap = std::map<Tag, std::string>;
//==========================================
size_t Mer::Endl::	current_line = 0;
TagStrMap	Mer::	TagStr{
	{ IMPORT,"IMPORT" },{ NAMESPACE,"NAMESPACE" },{STRUCT,"struct"},
	{ REF,"REF" },{ PROGRAM,"PROGRAME" },{ COMMA,"COMMA" },{ COLON,"COLON" },
	{ ID,"ID" },{ INTEGER,"INTEGER" },{ REAL,"REAL" } ,{ FUNCTION,"FUNCTION" },{ RETURN,"RETURN" },
	{ IF,"IF" },{ ELSE_IF,"ELSE_IF" },{ ELSE,"ELSE" },{ WHILE,"WHILE" },{ FOR,"FOR" },{ BREAK,"BREAK" },{ CONTINUE,"CONTINUE" },
	{ INTEGER_DECL,"INTEGER_DECL" },{ REAL_DECL,"REAL_DECL" },{ STRING_DECL,"STRING_DECL" },{ BOOL_DECL,"BOOL_DECL" },{VOID_DECL,"VOID_DECL"},
	{ PLUS,"PLUS" },{ MINUS,"MINUS" },{ MUL,"MUL" },{ DIV,"DIV" },
	{ GE,"GE" },{ GT,"GT" },{ LE,"LE" },{ LT,"LT" },{ EQ,"EQ" },{ NE,"NE" },
	{ AND,"AND" },{ OR,"OR" },{ NOT,"NOT" },{ GET_ADD,"GET_ADD" },
	{ LPAREN,"LPAREN" },{ RPAREN,"RPAREN" },
	{ DOT,"DOT" },{ BEGIN,"BEGIN" },{ END,"END" },
	{ SEMI,"SEMI" },{ ASSIGN,"ASSIGN" },{SADD,"SADD"},
	{ ENDL,"ENDL" },{ PRINT,"PRINT" },{ CAST,"CAST" },
	{ TTRUE,"TTRUE" },{ TFALSE,"TFALSE" },
};
TokenMap	Mer::	KeyWord{
	{ "import",new Token(IMPORT) },{ "namespace",new Token(NAMESPACE) },{"struct",new Token(STRUCT)},
	{ "if",new Token(IF) },{ "elif",new Token(ELSE_IF) },{ "else",new Token(ELSE) },
	{ "while",new Token(WHILE) },{ "break",new Token(BREAK) },{ "for",new Token(FOR) },
	{ "continue",new Token(CONTINUE) },
	{ "function",new Token(FUNCTION) },{ "return",new Token(RETURN) },
	{ "print",new Token(PRINT) },{ "cast",new Token(CAST) },{ "true",new Token(TTRUE) },
	{ "false",new Token(TFALSE) },
	{ "string",new Token(STRING_DECL) },{ "bool",new Token(BOOL_DECL) },
	{ "ref",new Token(REF) },{ "begin",new Token(BEGIN) },
	{ "end",new Token(END) },{ "real",new Token(REAL_DECL) },{ "void",new Token(VOID_DECL) },
	{ "int",new Token(INTEGER_DECL) },{ "program",new Token(PROGRAM) }
};
bool				is_function_args = false;
//==========================================
Token* Mer::		END_TOKEN = new Token(ENDOF);
TokenStream Mer::	token_stream;
Token* Mer::		parse_number(const std::string &str, size_t &pos)
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
Token*	Mer::		parse_word(const std::string &str, size_t &pos)
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
	auto result = Mer::KeyWord.find(ret);
	if (ret == "function")
		is_function_args = true;
	if (result != Mer::KeyWord.end())
	{
		return result->second;
	}
	auto id_result = Id::find(ret);
	if (id_result != nullptr)
		return id_result;
	else
		return new Id(ret);
}
Token*Mer::			parse_string(const std::string & str, size_t & pos)
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
void Mer::			build_token_stream(const std::string &content) {
	std::string tmp_str;
	token_stream.push_back(new Endl());
	for (size_t i = 0; i < content.size(); i++)
	{
		switch (content[i])
		{
		case '\'':
			token_stream.push_back(parse_string(content, i));
			break;
			case '\r':
		case '\n':
			token_stream.push_back(new Endl());
			break;
		case '{':
			if (!is_function_args)
			{
				Id::id_table().push_front(std::map<std::string, Id*>());
			}
			else
				is_function_args = false;
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
				break;
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
			}
			else
			{
				token_stream.push_back(new Token(ASSIGN));
			}
			break;
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
			if (is_function_args)
				Id::id_table().push_back(std::map<std::string, Id*>());
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
				break;
			}
			token_stream.push_back(new Token(MUL));
			break;
		case '/':
			if (i + 1 < content.size() && content[i + 1] == '/')
			{
				while (i + 1 < content.size() && content[i++] != '\n')
					continue;
				// in case that the uncorrect of line no
				i-=2;
				break;
			}
			else if (i + 1 < content.size() && content[i + 1] == '*')
			{
				i += 2;
				while (i < content.size())
				{
					if (content[i] == '\n')
					{
						token_stream.push_back(new Endl());
					}
					i++;
					if (content[i] == '*')
					{
						i++;
						if (i < content.size() && content[i] == '/')
							break;
					}
				}
				break;
			}
			else if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(new Token(SDIV));
				i++;
				break;
			}
			token_stream.push_back(new Token(DIV));
			break;
		case '+':
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(new Token(SADD));
				i++;
				break;
			}
			token_stream.push_back(new Token(PLUS));
			break;
		case '-':
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(new Token(SSUB));
				i++;
				break;
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
std::string Mer::	GIC()
{
	auto ret = Id::get_value(token_stream.this_token());
	token_stream.match(ID);
	return ret;
}
void TokenStream::	remove_tokens()
{
	int index = -1;
	// delete literal_const and endl value
	for (const auto &a : content)
	{
		index++;
		switch (a->get_tag())
		{
		case ENDL:
		case INTEGER:
		case REAL:
		case STRING:
		{
			auto tmp = a;
			content.erase(content.begin() + index);
			delete tmp;
			break;
		}
		default:
			break;
		}
	}
	Endl::current_line = 0;
	// delete Ids
	for (auto &a : Id::id_table())
	{
		for (auto &b : a)
		{
			delete b.second;
		}
		a.clear();
	}
	Id::id_table().clear();
	Id::id_table().push_back(std::map<std::string, Id*>());

	// delete container
	content.clear();
	//throw Error("HEY"+std::to_string(content.size()));
}
void TokenStream::	clear()
{
	remove_tokens();
	pos = 0;
}


std::string Endl::to_string()const
{
	return "\n";
}
size_t Endl::get_value(Token* tok)
{
	if (tok->get_tag() == ENDL)
	{
		return static_cast<Endl*>(tok)->line_no;
	}
	else
		throw Error("convert failed");
}