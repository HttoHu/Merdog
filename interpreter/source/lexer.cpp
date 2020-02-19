/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/lexer.hpp"
using namespace Mer;
using TokenMap = std::map<std::string, Token*>;
using TagStrMap = std::map<Tag, std::string>;
//==========================================
size_t Mer::Endl::current_line = 0;

TagStrMap	Mer::TagStr{
	{ IMPORT,"IMPORT" },{ NAMESPACE,"NAMESPACE" },{ STRUCT,"struct" },{NEW,"new"},{PTRVISIT,"PTRVISIT"},
	{ REF,"REF" },{ PROGRAM,"PROGRAME" },{ COMMA,"COMMA" },{ COLON,"COLON" },
	{ ID,"ID" },{ INTEGER,"INTEGER" },{ REAL,"REAL" } ,{ FUNCTION,"FUNCTION" },{ RETURN,"RETURN" },
	{ IF,"IF" },{ ELSE_IF,"ELSE_IF" },{ ELSE,"ELSE" },{ WHILE,"WHILE" },{ DO,"DO" } ,{ FOR,"FOR" },{ BREAK,"BREAK" },{ CONTINUE,"CONTINUE" },{SWITCH,"SWITCH"},
	{ INTEGER_DECL,"INTEGER_DECL" },{ REAL_DECL,"REAL_DECL" },{ STRING_DECL,"STRING_DECL" },{ BOOL_DECL,"BOOL_DECL" },{ VOID_DECL,"VOID_DECL" },{CHAR_DECL,"CHAR_DECL"},
	{ PLUS,"PLUS" },{ MINUS,"MINUS" },{ MUL,"MUL" },{ DIV,"DIV" },{MAKE,"make"},
	{ GE,"GE" },{ GT,"GT" },{ LE,"LE" },{ LT,"LT" },{ EQ,"EQ" },{ NE,"NE" },
	{ AND,"AND" },{ OR,"OR" },{ NOT,"NOT" },{ GET_ADD,"GET_ADD" },
	{ LPAREN,"LPAREN" },{ RPAREN,"RPAREN" },{ LSB,"LSB" },{ RSB,"RSB" },
	{ DOT,"DOT" },{ BEGIN,"BEGIN" },{ END,"END" },
	{ SEMI,"SEMI" },{ ASSIGN,"ASSIGN" },{ SADD,"SADD" },{ CASE,"CASE" },
	{ ENDL,"ENDL" },{ PRINT,"PRINT" },{ CAST,"CAST" },
	{ TTRUE,"TTRUE" },{ TFALSE,"TFALSE" },{NULLPTR,"NULLPTR"},{SIZEOF,"SIZEOF"},
};
TokenMap	Mer::BasicToken{
	{"+",new Token(PLUS)},{"-",new Token(MINUS)},{"*",new Token(MUL)},{"/",new Token(DIV)},{"=",new Token(ASSIGN)},
	{"+=",new Token(SADD)},{"-=",new Token(SSUB)},{"*=",new Token(SMUL)},{"/=",new Token(SDIV)},
	{"<",new Token(LT)},{"<=",new Token(LE)},{">",new Token(GT)},{">=",new Token(GE)},{"==",new Token(EQ)},
	{"!=",new Token(NE)},{"!",new Token(NOT)},{"&&",new Token(AND)},{"||",new Token(OR)},{":",new Token(COLON)},
	{",",new Token(COMMA)},{";",new Token(SEMI)},{".",new Token(DOT)},{"&",new Token(GET_ADD)},{"->",new Token(PTRVISIT)},
	{"[",new Token(LSB)},{"]",new Token(RSB)},{"(",new Token(LPAREN)},{")",new Token(RPAREN)},
	{"{",new Token(BEGIN)},{"}",new Token(END)},
	{ "using",new Token(IMPORT) },{ "namespace",new Token(NAMESPACE) },{ "struct",new Token(STRUCT) },
	{ "if",new Token(IF) },{ "elif",new Token(ELSE_IF) },{ "else",new Token(ELSE) },{"sizeof",new Token(SIZEOF)},
	{ "while",new Token(WHILE) },{ "break",new Token(BREAK) },{ "for",new Token(FOR) }, {"do",new Token(DO)},{"switch",new Token(SWITCH)}, {"case",new Token(CASE)},
	{ "continue",new Token(CONTINUE) },{"default",new Token(DEFAULT)},
	{ "function",new Token(FUNCTION) },{ "return",new Token(RETURN) },
	{ "new",new Token(NEW)},{"make",new Token(MAKE)},
	{ "cast",new Token(CAST) },{ "true",new Token(TTRUE) },
	{ "false",new Token(TFALSE) },
	{ "string",new Token(STRING_DECL) },{ "bool",new Token(BOOL_DECL) },
	{ "ref",new Token(REF) },{ "begin",new Token(BEGIN) },
	{ "end",new Token(END) },{ "real",new Token(REAL_DECL) },{ "void",new Token(VOID_DECL) },{"char",new Token(CHAR_DECL)},
	{ "int",new Token(INTEGER_DECL) },{ "program",new Token(PROGRAM) },{"nullptr",new Token(NULLPTR)}
};
bool				is_function_args = false;

std::map<char, char> escape_character_table = {
	{'r','\r'},{'n','\n'},{'b','\b'},{'t','\t'},{'a','\a'},{'0','\0'},{'\'','\''},{'\"','\"'}
};
char _convert_escape(char c)
{
	auto result = escape_character_table.find(c);
	if (result == escape_character_table.end())
	{
		throw LexerError(" illegal escape char");
	}
	return result->second;
}

//==========================================
Token* Mer::END_TOKEN = new Token(ENDOF);
TokenStream Mer::token_stream;
//get a word from content
std::string retrive_word(const std::string& str, size_t& pos)
{
	std::string ret;
	while (isalnum(str[pos]) || str[pos] == '_') ret += str[pos++];
	return ret;
}

void Mer::preprocess(const std::string& str, size_t& pos)
{
	// skip $
	pos++;
	std::string ins = retrive_word(str, pos);
	if (ins == "pre_input")
	{
		do
		{
			if (str[pos] == '\n' || str[pos] == '\r') {
				token_stream.push_back(new Endl());
			}
			input_buf += str[pos++];
		} while (str[pos] != '$');
	}
	else
		throw  LexerError("pre_ins: " + ins + " not defined yet.");
	pos++;
	std::string end_ins = retrive_word(str, pos);
	if (end_ins != "end")
		throw LexerError("illegal terminal word of end preprocess " + end_ins);
	my_stringstream.str(input_buf);
	// back to the last char in case of the lexer eating \n or \r which may lead to the wrong line number.
	pos--;
}

Token* Mer::parse_number(const std::string& str, size_t& pos)
{
	int ret = 0;
	for (; pos < str.size(); pos++)
	{
		if (isdigit(str[pos]))
		{
			ret = ret * 10 + (str[pos] - 48);
		}
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
				tmp = tmp + tmp2 * (str[pos] - (size_t)48);
			}
			else
			{
				return new Real((double)ret + tmp);
			}
		}
	}
	return new Integer(ret);
}

Token* Mer::parse_word(const std::string& str, size_t& pos)
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
	auto result = Mer::BasicToken.find(ret);
	if (ret == "function")
		is_function_args = true;
	if (result != Mer::BasicToken.end())
	{
		return result->second;
	}
	auto id_result = Id::find(ret);
	if (id_result != nullptr)
		return id_result;
	else
	{
		return new Id(ret);
	}
}
Token* Mer::parse_string(const std::string& str, size_t& pos)
{
	std::string value;
	// jump over '
	pos++;
	for (; pos < str.size(); pos++)
	{
		if (str[pos] == '\"')
			break;
		if (str[pos] == '\\')
		{
			pos++;
			if (pos >= str.size())
				throw LexerError("out of range");
			char tmp = _convert_escape(str[pos]);
			value += tmp;
		}
		else
			value += str[pos];
	}
	return new String(value);
}
void Mer::build_token_stream(const std::string& content) {
	std::string tmp_str;
	token_stream.push_back(new Endl());
	for (size_t i = 0; i < content.size(); i++)
	{
		switch (content[i])
		{
		case '$':
			preprocess(content, i);
			break;
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
				Id::id_table().push_front(std::map<std::string, Id*>());
			else
				is_function_args = false;
			token_stream.push_back(BasicToken[std::string(1, content[i])]);
			break;
		case '}':
			for (auto& a : Id::id_table().front())
			{
				token_stream._rem_tok_vec.push_back(a.second);
			}
			Id::id_table().pop_front();

			token_stream.push_back(BasicToken["}"]);
			break;
		case ',':
			token_stream.push_back(BasicToken[","]);
			break;
		case '&':
			if (i + 1 < content.size() && content[i + 1] == '&')
			{
				token_stream.push_back(BasicToken["&&"]);
				i++;
				break;
			}
			else
				token_stream.push_back(BasicToken["&"]);
			break;
		case '|':
			if (i + 1 < content.size() && content[i + 1] == '|')
			{
				token_stream.push_back(BasicToken["||"]);
				i++;
			}
			break;
		case '+':
			if (i + 1 < content.size() && content[i + 1] == '+')
				throw LexerError("merdog doesn't support inc operation, please replaced it with +=1");
		case '>':
		case '<':
		case '=':
		case '!':
		case '*':
		{
			std::string str = std::string(1, content[i]);
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				str += content[++i];
			}
			token_stream.push_back(BasicToken[str]);
			break;
		}
		case ';':
		case ':':
		case '.':
		case '[':
		case ']':
		case ')':
			token_stream.push_back(BasicToken[std::string(1, content[i])]);
			break;
		case '\t':
		case ' ':
			break;
		case '(':
			if (is_function_args)
				Id::id_table().push_back(std::map<std::string, Id*>());
			token_stream.push_back(BasicToken["("]);
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
					if (content[i] == '\n' || content[i] == '\r')
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
				token_stream.push_back(BasicToken["/="]);
				i++;
				break;
			}
			token_stream.push_back(BasicToken["/"]);
			break;

		case '-':
			if (i + 1 < content.size() && content[i + 1] == '=')
			{
				token_stream.push_back(BasicToken["-="]);
				i++;
				break;
			}
			else if (i + 1 < content.size() && content[i + 1] == '>')
			{
				token_stream.push_back(BasicToken["->"]);
				i++;
				break;
			}
			else if (i + 1 < content.size() && content[i + 1] == '-')
				throw LexerError("merdog doesn't support dec operation, please replaced it with -=1");
			token_stream.push_back(BasicToken["-"]);
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

std::string Mer::get_this_id_string_value()
{
	auto ret = Id::get_value(token_stream.this_token());
	token_stream.match(ID);
	return ret;
}
Token* Mer::TokenStream::next_token()
{
	if (pos + 1 < content.size())
		return content[pos + 1];
	else
		throw Error("token_stream out of range");
}
void Mer::TokenStream::add(Token* tok)
{
	content.insert(content.begin() + pos + 1, tok);
}
void Mer::TokenStream::advance()
{
	pos++;
	if (pos >= content.size())
	{
		content.push_back(END_TOKEN);
		throw Error("to the end of token_stream");
	}
}
void Mer::TokenStream::match(Tag t)
{
	// to check the token whether it is right, and if matched call advance, or throw an error.
	// example: match(PLUS); 
	if (this_token()->get_tag() == Mer::Tag::ENDL)
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
		case CHAR_LIT:
		case INTEGER:
		case REAL:
		case STRING:
		{
			auto tmp = a;
			delete tmp;
			break;
		}
		default:
			break;
		}
	}
	content.clear();
	Endl::current_line = 0;
	// delete Ids
	for (auto& a : Id::id_table())
	{
		for (auto& b : a)
		{
			delete b.second;
		}
	}
	Id::id_table().clear();
	Id::id_table().push_back(std::map<std::string, Id*>());

	// delete container
	content.clear();
}
void TokenStream::clear()
{
	for (auto& a : _rem_tok_vec)
		delete a;
	_rem_tok_vec.clear();
	remove_tokens();

	pos = 0;
}

Token* Mer::parse_char(const std::string& str, size_t& pos)
{
	// ignore \'
	pos++;
	if (str[pos] == '\\')
	{
		char result = _convert_escape(str[++pos]);
		if (str[++pos] != '\'')
			throw LexerError("illegal escape character!");
		return new CharToken(result);
	}
	char result = str[pos++];
	if (str[pos] != '\'')
		throw LexerError("illegal character!");
	return new CharToken(result);
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
		throw LexerError("convert failed");
}