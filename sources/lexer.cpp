#include "../include/lexer.hpp"
#include "../include/environment.hpp"
using namespace Merdog;

int Merdog::index=0;

bool _is_operator(char ch)
{
	switch (ch)
	{
	case '+':case '-':case '*':case '/':case '&':case '|':case ':':case'=':case'<':case'>':case'[':case']':case'(':case')':
	case';':case ',':case '.':case'!':case'#':
		return true;
	default:
		return false;
	}
}

bool Merdog::is_operator(TokenType t)
{
	switch (t)
	{
	case Merdog::New:
	case Merdog::Or:
	case Merdog::And:
	case Merdog::Not:
	case Merdog::Eq:
	case Merdog::Ne:
	case Merdog::Ge:
	case Merdog::Lt:
	case Merdog::Gt:
	case Merdog::Le:
	case Merdog::Assign:
	case Merdog::SAdd:
	case Merdog::SSub:
	case Merdog::SMul:
	case Merdog::SDiv:
	case Merdog::Add:
	case Merdog::Sub:
	case Merdog::Mul:
	case Merdog::Div:
	case Merdog::PP:
	case Merdog::MM:
		return true;
	default:
		return false;
	}
}

void _read_string() {
	std::string value;
	if (Environment::file_content()[index] == '\"')
		index++;
	else
		throw Error("a string-literal must start with\"");
	for (; Environment::file_content()[index] != '\"'; index++)
	{
		if (index >= Environment::file_content().size())
		{
			throw Error("string-literal isn't matched");
		}
		if (Environment::file_content()[index] == '\\')
		{
			if (index >= Environment::file_content().size() - 1)
			{
				throw Error("string-literal isn't matched");
			}
			index++;
			switch (Environment::file_content()[index]) {
			case 'n':
				value += '\n';
				break;
			case 't':
				value += '\t';
				break;
			case '\"':
				value += '\"';
				break;
			case '\'':
				value += '\'';
				break;
			case '\\':
				value += '\\';
				break;
			}
		}
		else
			value += Environment::file_content()[index];
	}
	index++;
	Environment::token_stream().push_back(new Literal(value));
}
void _read_char()
{
	char ch = 0;
	if (Environment::file_content()[index++] != '\'')
	{
		throw Error("a char-literal must start with\'");
	}
	if (Environment::file_content()[index] == '\\')
	{
		index++;
		switch (Environment::file_content()[index])
		{
		case 'n':
			ch = '\n';
			break;
		case 't':
			ch = '\t';
			break;
		case '\\':
			ch = '\\';
			break;
		case '\'':
			ch = '\'';
			break;
		case '\"':
			ch = '\"';
			break;
		default:
			throw Error("unexcepted escape char!");
			break;
		}
		index++;
		Environment::token_stream().push_back(new Literal(ch));
		if (Environment::file_content()[index] == '\'')
		{
			index++;
			return;
		}
		else
		{
			throw Error("char-literal must be a single char.");
		}
	}
	else
	{
		ch = Environment::file_content()[index++];
		Environment::token_stream().push_back(new Literal(ch));
		if (Environment::file_content()[index] == '\'')
		{
			index++;
			return;
		}
		else
		{
			throw Error("char-literal must be a single char.");
		}
	}
}
void _read_symbol()
{
	std::string tmp(1, Environment::file_content()[index]);
	if (index < Environment::file_content().size() - 1 && _is_operator(Environment::file_content()[index + 1]))
	{
		tmp += Environment::file_content()[index + 1];
	}
	auto result = Token::symbol_map().find(tmp);
	if (result == Token::symbol_map().end())
	{
		Environment::token_stream().push_back(
			Token::symbol_map().find(std::string(1, Environment::file_content()[index++]))->second 
		);
		return;
	}
	index += tmp.size();
	Environment::token_stream().push_back(result->second);
}
void _read_number()
{
	int64_t intPart = 0;
	bool isN = Environment::file_content()[index] == '-';
	if (isN)
	{
		if (!isdigit(Environment::file_content()[index + 1]))
		{
			_read_symbol();
			return;
		}
		if (Environment::token_stream().back()->get_token_type() == TLiteral ||Environment::token_stream().back()->get_token_type() == TId)
			Environment::token_stream().push_back(new Token(Add));
		index++;
	}
	while (isdigit(Environment::file_content()[index]))
	{
		intPart = intPart * 10 + (Environment::file_content()[index++] - 48);
	}
	if (Environment::file_content()[index] == 'L')
	{
		index++;
		if (isN)
			intPart = -intPart;
		Environment::token_stream().push_back(new Literal(intPart));
		return;
	}
	if (Environment::file_content()[index] != '.')
	{
		if (isN)
			intPart = -intPart;
		Environment::token_stream().push_back(new Literal(static_cast<int32_t>(intPart)));
		return;
	}
	index++;
	double v = (double)intPart;
	double v2 = 0.1;
	while (isdigit(Environment::file_content()[index]))
	{
		v += v2*(Environment::file_content()[index++] - 48);
		v2 /= 10;
	}
	if (isN)
		v = -v;
	Environment::token_stream().push_back(new Literal(v));
}
void _read_id()
{
	std::string word;
	while ((isalnum(Environment::file_content()[index]) 
		|| Environment::file_content()[index] == '_') 
		&& index < Environment::file_content().size())
	{
		word += Environment::file_content()[index++];
	}
	auto result = Token::keyword_map().find(word);
	if (result == Token::keyword_map().end())
	{
		Id *tmp = Id::find_id(word);
		if (tmp == nullptr)
		{
			tmp = new Id(word);
			Id::id_table().back().insert({ word, tmp });
		}
		Environment::token_stream().push_back(tmp);
		return;
	}
	Environment::token_stream().push_back(result->second);

}

void Merdog::scan()
{
	Environment::token_stream().push_back(new LineNo());
	int strain = 0;
	while (index < Environment::file_content().size())
	{
		switch (Environment::file_content()[index])
		{
		case ';':
			Environment::token_stream().push_back(new Token(EndStmt));
			index++;
			continue;
		case '-':
			_read_number();
			continue;
			/* note that if you input 
			issue:
			(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((([]] then Merdog's scanner can not notice any errors.
			however I don't intent to fix it.*/
		case '(':
			Environment::token_stream().push_back(new Token(LBk));
			strain += 1;
			index++;
			continue;
		case ')':
			Environment::token_stream().push_back(new Token(RBk));
			strain -= 1;
			index++;
			continue;
		case '[':
			Environment::token_stream().push_back(new Token(LMk));
			strain += 100;
			index++;
			continue;
		case ']':
			Environment::token_stream().push_back(new Token(RMk));
			strain -= 100;
			index++;
			continue;
		case '{':
			Environment::token_stream().push_back(new Token(Begin));
			Id::id_table().push_back(std::map<std::string, Id*>());
			index++;
			strain += 100000;
			continue;
		case '}':
			Environment::token_stream().push_back(new Token(End));
			Id::id_table().pop_back();
			strain -= 100000;
			index++;
			continue;
		case '\'':
			_read_char();
			continue;
		case '\n':
			Environment::token_stream().push_back(new LineNo());
			index++;
			continue;
		case '\"':
			_read_string();
			continue;
		case '/':
			index++;
			if (Environment::file_content()[index] != '/')
			{
				index--;
				break;
			}
			else
			{
				while (index != Environment::file_content().size() && Environment::file_content()[index++] != '\n')
					;
				continue;
			}
			break;
		case '\t':
		case ' ':
			index++;
			continue;
		default:
			break;
		}
		if (isdigit(Environment::file_content()[index]))
			_read_number();
		else if (_is_operator(Environment::file_content()[index]))
			_read_symbol();
		else if (isalpha(Environment::file_content()[index]) || Environment::file_content()[index] == '_')
			_read_id();
		else
		{
			throw Error(std::string(1, Environment::file_content()[index]) + " unkonwn char");
		}
	}
	if (strain != 0)
		throw Error("brackets are not balance!");
}
