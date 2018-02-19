#pragma once
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "error.hpp"
namespace Mer
{
	enum Tag
	{
		PRINT,
		SPLUS,SMINUS,SMUL,SDIV,
		INTEGER_DECL, REAL_DECL,STRING_DECL,BOOL_DECL,
		PROGRAM,
		GE,LE,GT,LT,NE,EQ,
		IF,ELSE_IF,ELSE,WHILE,FOR,BREAK,CONTINUE,
		NOT,AND,OR,
		VAR, BEGIN, END, SEMI, DOT, COMMA,
		ID, INTEGER, REAL, COLON,
		PLUS, MINUS, MUL, DIV, ASSIGN,
		TRUE,FALSE,
		LPAREN, RPAREN,
		ENDOF, ENDL,
		STRING,
	};
	extern std::map<Tag, std::string> TagStr;
	class Token
	{
	public:
		Token(Tag t) :token_type(t) {}
		Tag get_tag()const { return token_type; }

		bool check(Tag t) {
			return token_type == t;
		}
		virtual std::string to_string()const {
			return "<" + TagStr[token_type] + ">";
		}
		virtual ~Token() {}
	private:
		Tag token_type;
	};
	extern std::map<std::string, Token*>KeyWord;
	extern Token* END_TOKEN;
	// extern std::map<std::string,Token*> key_map;
	class Id final :public Token
	{
	public:
		Id(const std::string &str) :Token(ID), id_name(str)
		{
			id_table().front().insert({ id_name ,this });
		}
		static std::string get_value(Token *tok)
		{
			if (tok->check(ID))
				return static_cast<Id*>(tok)->id_name;
			throw Mer::Error("type-convert failed(Token can't convert to Id).");
		}
		static std::vector<std::map<std::string, Id*>> &id_table()
		{
			static std::vector<std::map<std::string, Id*>> ret(1);
			return ret;
		}
		static Id* find(std::string str)
		{
			for (int i = (int)id_table().size() - 1; i >= 0; i--)
			{
				auto result = id_table()[i].find(str);
				if (result == id_table()[i].end())
					continue;
				return result->second;
			}
			return nullptr;
		}
		std::string to_string()const override {
			return "<Id:" + id_name + ">";
		}
	private:
		std::string id_name;
	};
	class Integer :public Token
	{
	public:
		Integer(int64_t n) :Token(INTEGER), value(n) {}
		static int64_t get_value(Token *tok)
		{
			if (tok->check(INTEGER))
				return static_cast<Integer*>(tok)->value;
			throw Mer::Error("type-convert failed(Token can't convert to Integer).");
		}
		std::string to_string()const override {
			return "<Integer:" + std::to_string(value) + ">";
		}
	private:
		int64_t value;
	};
	class Real :public Token
	{
	public:
		Real(double d) :Token(REAL), value(d) {}
		static double get_value(Token *tok)
		{
			return static_cast<Real*>(tok)->value;
			throw Mer::Error("type-convert failed(Token can't convert to Real).");
		}
		std::string to_string()const override
		{
			return "<Real:" + std::to_string(value) + ">";
		}
	private:
		double value;
	};
	class TokenStream
	{
	public:
		TokenStream() = default;
		void push_back(Mer::Token* tok)
		{
			content.push_back(tok);
		}
		void pop_back()
		{
			content.pop_back();
		}
		Token* this_token()
		{
			while (content[pos]->get_tag() == ENDL)
				advance();
			return content[pos];
		}
		Tag this_tag()
		{
			return this_token()->get_tag();
		}
		Token* get_next_token()
		{
			auto tmp = content[pos];
			advance();
			return tmp;
		}
		void advance()
		{
			pos++;
			if (pos >= content.size())
			{
				content.push_back(END_TOKEN);
			}
		}
		void next()
		{
			if (this_token()->get_tag() == ENDL)
			{
				advance();
				next();
			}
			advance();
		}
		void match(Tag t)
		{
			// to check the token whether it is right, and if matched call advance, or throw an error.
			// example: match(PLUS); 
			if (this_token()->get_tag() == ENDL)
			{
				advance();
				match(t);
			}
			else if (this_token()->get_tag() == t)
				advance();
			else
				throw Error(this_token()->to_string() + " not match with " + TagStr[t]);
		}
		void print()
		{
			for (const auto &a : content)
				std::cout << a->to_string();
		}
		std::vector<Token*>& _get_content()
		{
			return content;
		}
		size_t _get_pos()
		{
			return pos;
		}
	private:
		std::vector<Token*> content;
		size_t pos = 0;
	};
	class Endl :public Token
	{
	public:
		Endl() :Token(ENDL)
		{
			static size_t current_line = 0;
			line_no = ++current_line;
		}
		static size_t get_value(Token* tok)
		{
			if (tok->get_tag() == ENDL)
			{
				return static_cast<Endl*>(tok)->line_no;
			}
			else
				throw Error("convert failed");
		}

		std::string to_string()const override
		{
			return "\n";
		}
	private:
		size_t line_no;
	};
	class String :public Token
	{
	public:
		static std::string get_value(Token *tok)
		{
			return static_cast<String*>(tok)->value;
		}
		String(const std::string&str):Token(STRING),value(str) {}
		std::string to_string()const override
		{
			return "<str:" + value + ">";
		}
	private:
		std::string value;
	};
    Token* parse_number(const std::string &str,size_t &pos);
    Token* parse_word(const std::string &str,size_t &pos);
	Token* parse_string(const std::string &str, size_t &pos);
    void build_token_stream(const std::string &content);
    extern TokenStream token_stream;
}