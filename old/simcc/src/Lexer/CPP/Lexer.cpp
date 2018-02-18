#include "../HPP/Lexer.hpp"
using namespace Simcc::Lexer;
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
void Lexer::read_string() {
	std::string value;
	if (content[index] == '\"')
		index++;
	else
		throw std::runtime_error("a string-literal must start with\"");
	for (; content[index] != '\"'; index++)
	{
		if (index >= content.size())
		{
			throw std::runtime_error("string-literal isn't matched");
		}
		if (content[index] == '\\')
		{
			if (index >= content.size() - 1)
			{
				throw std::runtime_error("string-literal isn't matched");
			}
			index++;
			switch (content[index]) {
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
			value += content[index];
	}
	index++;
	token_stream.push_back(new Simcc::Lexer::VString(value));
}
void Simcc::Lexer::Lexer::read_char()
{
	char ch = 0;
	if (content[index++] != '\'')
	{
		throw std::runtime_error("a char-literal must start with\'");
	}
	if (content[index] == '\\')
	{
		index++;
		switch (content[index])
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
			throw std::runtime_error("unexcepted escape char!");
			break;
		}
		index++;
		token_stream.push_back(new VChar(ch));
		if (content[index] == '\'')
		{
			index++;
			return;
		}
		else
		{
			throw std::runtime_error("char-literal must be a single char.");
		}
	}
	else
	{
		ch = content[index++];
		token_stream.push_back(new VChar(ch));
		if (content[index] == '\'')
		{
			index++;
			return;
		}
		else
		{
			throw std::runtime_error("char-literal must be a single char.");
		}
	}
}
void Simcc::Lexer::Lexer::read_number()
{
	int64_t intPart = 0;
	bool isN = content[index] == '-';
	if (isN)
	{
		if (!isdigit(content[index + 1]))
		{
			read_symbol();
			return;
		}
		if (token_stream.back()->get_tag() == TLiteralInt || token_stream.back()->get_tag() == TLiteralDouble ||
			token_stream.back()->get_tag() == TLiteralLong || token_stream.back()->get_tag() == TLiteralChar||
			token_stream.back()->get_tag() ==Id)
			token_stream.push_back(new Token(Add));
		index++;
	}
	while (isdigit(content[index]))
	{
		intPart = intPart * 10 + (content[index++] - 48);
	}
	if (content[index] == 'L')
	{
		index++;
		if (isN)
			intPart = -intPart;
		token_stream.push_back(new VLong(intPart));
		return;
	}
	if (content[index] != '.')
	{
		if (isN)
			intPart = -intPart;
		token_stream.push_back(new VInt(static_cast<int32_t>(intPart)));
		return;
	}
	index++;
	double v = (double)intPart;
	double v2 = 0.1;
	while (isdigit(content[index]))
	{
		v += v2*(content[index++] - 48);
		v2 /= 10;
	}
	if (isN)
		v = -v;
	token_stream.push_back(new VDouble(v));
}
void Simcc::Lexer::Lexer::read_word()
{
	std::string word;
	while ((isalnum(content[index]) || content[index] == '_') && index < content.size())
	{
		word += content[index++];
	}
	auto result = keyword_map().find(word);
	if (result == keyword_map().end())
	{
		TId *tmp = TId::find_id(word);
		if (tmp == nullptr)
		{ 
			tmp = new TId(word);
			TId::id_table.back().insert({ word, tmp });
		}
		token_stream.push_back(tmp);
		return;
	}
	token_stream.push_back(new Token(result->second));

}
void Simcc::Lexer::Lexer::read_symbol()
{
	std::string tmp(1, content[index]);
	if (index < content.size() - 1 && _is_operator(content[index + 1]))
	{
		tmp += content[index + 1];
	}
	auto result = symbol_map().find(tmp);
	if (result == symbol_map().end())
	{
		token_stream.push_back(new Token(symbol_map().find(std::string(1, content[index++]))->second));
		return;
	}
	index += tmp.size();
	token_stream.push_back(new Token(result->second));
}
void Lexer::debug()
{
	for (const auto & a : token_stream)
	{
		std::cout << a->to_string();
	}
}

void Simcc::Lexer::Lexer::init_token_stream()
{
	while (index < content.size())
	{
		switch (content[index])
		{
		case '-':
			read_number();
			continue;
		case '{':
			token_stream.push_back(new Token(Tag::BlockBegin));
			TId::id_table.push_back(std::map<std::string,TId*>());
			index++;
			continue;
		case '}':
			token_stream.push_back(new Token(Tag::BlockEnd));
			TId::id_table.pop_back();
			index++;
			continue;
		case '\'':
			read_char();
			continue;
		case '\n':
			token_stream.push_back(new EndLine());
			index++;
			continue;
		case '\"':
			read_string();
			continue;
		case '/':
			index++;
			if (content[index] != '/')
			{
				index--;
				break;
			}
			else
			{
				while (index != content.size() && content[index++] != '\n')
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
		if (isdigit(content[index]))
			read_number();
		else if (_is_operator(content[index]))
			read_symbol();
		else if (isalpha(content[index]) || content[index] == '_')
			read_word();
		else
		{
			throw std::runtime_error(std::string(1, content[index]) + " unkonwn char");
		}
	}
	token_stream.push_back(new EndLine());
}
