#include "../include/lexer.hpp"
using namespace Mer;

std::map<Tag,std::string> Mer::TagStr{
    {ID,"ID"},{INTEGER,"INTEGER"},{PLUS,"PLUS"},{MINUS,"MINUS"},{MUL,"MUL"},{DIV,"DIV"},
	{LPAREN,"LPAREN"},{RPAREN,"RPAREN"}
};
Token* Mer::END_TOKEN=new Token(END);
TokenStream Mer::token_stream;
Token * Mer::parse_interger(const std::string &str,size_t &pos)
{
    int64_t ret=0;
    for (;pos<str.size();pos++) 
    {
		if (isdigit(str[pos]))
			ret = ret * 10 + (str[pos] - 48);
		else
		{
			break;
		}
    }

    return new Integer(ret);
}
Token *Mer::parse_id(const std::string &str,size_t &pos)
{
    std::string ret;
    bool first_char=true;
    for (;pos<str.size();pos++)
    {
        if(first_char)
        {
            first_char=false;
            if(isalpha(str[pos])||str[pos]=='_')
                ret+=str[pos];
            else
                throw Error("a word must begin with alpha or '_' ");
            continue;
        }
        else
        {
            if(isalnum(str[pos])||str[pos]=='_')
                ret+=str[pos];
            else
            {
                pos--;
                break;
            }
        }
    }
    return new Id(ret);
}
void Mer::build_token_stream(const std::string &content)
{
    std::string tmp_str;
    for (size_t i=0;i<content.size();i++)
    {
        switch(content[i])
        {
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
                token_stream.push_back(parse_interger(content,i));
				i--;
                break;
            default:
                token_stream.push_back(parse_id(content,i));
                break;
        }
    }
}