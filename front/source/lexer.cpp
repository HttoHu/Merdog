/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/lexer.hpp"
using namespace mer;
using TokenMap = std::map<std::string, Token*>;
using TagStrMap = std::map<Tag, std::string>;
//==========================================
int mer::Endl::current_line = 0;
std::map<char, char> mer::escape_character_table = {
	{'r','\r'},{'n','\n'},{'b','\b'},{'t','\t'},{'a','\a'},{'0','\0'}
};
char _convert_escape(char c)
{
	auto result = escape_character_table.find(c);
	if (result == escape_character_table.end())
	{
		throw Error(" illegal escape char");
	}
	return result->second;
}

TagStrMap	mer::TagStr{
	{ IMPORT,"IMPORT" },{ NAMESPACE,"NAMESPACE" },{ STRUCT,"struct" },{NEW,"new"},{PTRVISIT,"PTRVISIT"},
	{ REF,"REF" },{ PROGRAM,"PROGRAME" },{ COMMA,"COMMA" },{ COLON,"COLON" },
	{ ID,"ID" },{ INTEGER,"INTEGER" },{ REAL,"REAL" } ,{ FUNCTION,"FUNCTION" },{ RETURN,"RETURN" },
	{ IF,"IF" },{ ELSE_IF,"ELSE_IF" },{ ELSE,"ELSE" },{ WHILE,"WHILE" },{ DO,"DO" } ,{ FOR,"FOR" },{ BREAK,"BREAK" },{ CONTINUE,"CONTINUE" },{SWITCH,"SWITCH"},
	{ INTEGER_DECL,"INTEGER_DECL" },{ REAL_DECL,"REAL_DECL" },{ STRING_DECL,"STRING_DECL" },{ BOOL_DECL,"BOOL_DECL" },{ VOID_DECL,"VOID_DECL" },{CHAR_DECL,"CHAR_DECL"},
	{ PLUS,"PLUS" },{ MINUS,"MINUS" },{ MUL,"MUL" },{ DIV,"DIV" },{DELETE,"DELETE"},
	{ GE,"GE" },{ GT,"GT" },{ LE,"LE" },{ LT,"LT" },{ EQ,"EQ" },{ NE,"NE" },
	{ AND,"AND" },{ OR,"OR" },{ NOT,"NOT" },{ GET_ADD,"GET_ADD" },
	{ LPAREN,"LPAREN" },{ RPAREN,"RPAREN" },{ LSB,"LSB" },{ RSB,"RSB" },
	{ DOT,"DOT" },{ BEGIN,"BEGIN" },{ END,"END" },
	{ SEMI,"SEMI" },{ ASSIGN,"ASSIGN" },{ SADD,"SADD" },
	{ ENDL,"ENDL" },{ PRINT,"PRINT" },{ CAST,"CAST" },
	{ TTRUE,"TTRUE" },{ TFALSE,"TFALSE" },{NULLPTR,"NULLPTR"}, {ENDOF,"END_FILE"}
};
TokenMap	mer::KeyWord{
	{ "import",new Token(IMPORT) },{ "namespace",new Token(NAMESPACE) },{ "struct",new Token(STRUCT) },
	{ "if",new Token(IF) },{ "elif",new Token(ELSE_IF) },{ "else",new Token(ELSE) },
	{ "while",new Token(WHILE) },{ "break",new Token(BREAK) },{ "for",new Token(FOR) }, {"do",new Token(DO)},{"switch",new Token(SWITCH)}, {"case",new Token(CASE)},
	{ "continue",new Token(CONTINUE) },{"default",new Token(DEFAULT)},
	{ "function",new Token(FUNCTION) },{ "return",new Token(RETURN) },
	{ "new",new Token(NEW)},{"delete",new Token(DELETE)},
	{ "print",new Token(PRINT) },{ "cast",new Token(CAST) },{ "true",new Token(TTRUE) },
	{ "false",new Token(TFALSE) },
	{ "string",new Token(STRING_DECL) },{ "bool",new Token(BOOL_DECL) },
	{ "ref",new Token(REF) },{ "begin",new Token(BEGIN) },
	{ "end",new Token(END) },{ "real",new Token(REAL_DECL) },{ "void",new Token(VOID_DECL) },{"char",new Token(CHAR_DECL)},
	{ "int",new Token(INTEGER_DECL) },{ "program",new Token(PROGRAM) },{"nullptr",new Token(NULLPTR)}
};
bool				is_function_args = false;
//==========================================
Token* mer::END_TOKEN = new Token(ENDOF);
TokenStream mer::token_stream;

Token* mer::parse_char(const std::string& str, int& pos)
{
	// ignore \'
	pos++;
	if (str[pos] == '\\')
	{
		char result = _convert_escape(str[++pos]);
		if (str[pos] != '\'')
			throw Error("illegal escape character!");
		return new Char(result);
	}
	char result = str[pos++];
	if (str[pos] != '\'')
		throw Error("illegal character!");
	return new Char(result);
}

Token* mer::parse_number(const std::string& str, int& pos)
{
	int ret = 0;
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
				tmp = tmp + tmp2 * static_cast<double>((int)str[pos] - 48);
			}
			else
			{
				return new Real((double)ret + tmp);
			}
		}
	}
	return new Integer(ret);
}

std::string mer::tag_to_sign(Tag t)
{
	switch (t)
	{
	case ASSIGN:
		return "=";
	case SADD:
		return "+=";
	case SSUB:
		return "-=";
	case SMUL:
		return "*=";
	case SDIV:
		return "/=";
	case PLUS:
		return "+";
	case MINUS:
		return "-";
	case DIV:
		return "/";
	case MUL:
		return "*";
	default:
		throw Error("unkown sign!");
	}
}

Token* mer::parse_word(const std::string& str, int& pos)
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
				throw Error("illegal Symbol");
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
	auto result = mer::KeyWord.find(ret);
	if (ret == "function")
		is_function_args = true;
	if (result != mer::KeyWord.end())
	{
		return result->second;
	}
	auto id_result = Id::find(ret);
	if (id_result != nullptr)
		return id_result;
	else
		return new Id(ret);
}
Token* mer::parse_string(const std::string& str, int& pos)
{
	std::string value;
	if (str[pos] == '\"')
		pos++;
	else
		throw Error("a string-literal must start with\"");
	for (; pos < str.size(); pos++)
	{
		if (str[pos] == '\"')
			break;
		if (str[pos] == '\\')
		{
			pos++;
			if (pos >= str.size())
				throw Error("out of range");
			switch (str[pos])
			{
			case 'a':
				value += '\a';
				break;
			case 'b':
				value += '\b';
				break;
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
void mer::build_token_stream(const std::string& content) {
	std::string tmp_str;
	token_stream.push_back(new Endl());
	for (int i = 0; i < content.size(); i++)
	{
		switch (content[i])
		{
		case '\'':
			token_stream.push_back(parse_char(content, i));
			break;
		case '\"':
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
		case '[':
			token_stream.push_back(new Token(LSB));
			break;
		case ']':
			token_stream.push_back(new Token(RSB));
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
				i -= 2;
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
			else if (content[i + 1] == '>')
			{
				token_stream.push_back(new Token(PTRVISIT));
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

std::string mer::GIC()
{
	auto ret = Id::get_value(token_stream.this_token());
	token_stream.match(ID);
	return ret;
}
void mer::TokenStream::back_to(index_type index)
{
	pos = index;
}
index_type mer::TokenStream::current_index()
{
	return pos;
}
Token* mer::TokenStream::next_token()
{
	if (pos + 1 < content.size())
		return content[pos + 1];
	else
		throw Error("token_stream out of range");
}
void mer::TokenStream::back()
{
	if (this_token()->get_tag() == Tag::ENDL || this_token()->get_tag() == Tag::EPT)
	{
		dec();
		back();
	}
	dec();
}
void mer::TokenStream::add(Token* tok)
{
	content.insert(content.begin() + pos + 1, tok);
}
void mer::TokenStream::advance()
{
	pos++;
	if (pos >= content.size())
	{
		content.push_back(END_TOKEN);
		throw Error("to the end of token_stream");
	}
}
void mer::TokenStream::next()
{
	if (this_token()->get_tag() == Tag::ENDL || this_token()->get_tag() == Tag::EPT)
	{
		advance();
		next();
	}
	advance();
}
void mer::TokenStream::match(Tag t)
{
	// to check the token whether it is right, and if matched call advance, or throw an error.
	// example: match(PLUS); 
	if (this_token()->get_tag() == mer::Tag::ENDL)
	{
		advance();
		match(t);
	}
	else if (this_token()->get_tag() == t)
		advance();
	else
		throw Error(this_token()->to_string() + " not match with " + TagStr[t]);
}
void TokenStream::remove_tokens()
{
	int index = -1;
	// delete literal_const and endl value
	for (const auto& a : content)
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
	for (auto& a : Id::id_table())
	{
		for (auto& b : a)
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
void TokenStream::clear()
{
	remove_tokens();
	pos = 0;
}


std::string Endl::to_string()const
{
	return "\n";
}
int Endl::get_value(Token* tok)
{
	if (tok->get_tag() == ENDL)
	{
		return static_cast<Endl*>(tok)->line_no;
	}
	else
		throw Error("convert failed");
}

char mer::Char::get_value(Token* tok)
{
	if (tok->get_tag() != Tag::CHAR)
		throw Error(tok->to_string() + " is not a string");
	return static_cast<Char*>(tok)->ch;
}
