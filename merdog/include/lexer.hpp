/*
*		Inspired by
*		https://ruslanspivak.com/lsbasi-part10/
*		Ruslan's Blog
*		C++ Version.
*		Yuantao Hu 2018
*		Email :Huyuantao@outlook.com
*/
#pragma once
#include <deque>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "error.hpp"
namespace Mer
{
	enum Tag
	{
		EPT,
		IMPORT, NAMESPACE, STRUCT,
		PRINT, CAST,
		SADD, SSUB, SMUL, SDIV, GET_ADD,
		VOID_DECL, INTEGER_DECL, REAL_DECL, STRING_DECL, BOOL_DECL,
		PROGRAM,
		GE, LE, GT, LT, NE, EQ,
		FUNCTION, RETURN,
		IF, ELSE_IF, ELSE, DO,WHILE, FOR, BREAK, CONTINUE,SWITCH,CASE,
		DEFAULT,
		NEW,DELETE,
		NOT, AND, OR,
		REF, BEGIN, END, SEMI, DOT, COMMA,
		ID, INTEGER, REAL, COLON,
		PLUS, MINUS, MUL, DIV, ASSIGN,
		TTRUE, TFALSE,
		LPAREN, RPAREN,LSB,RSB,
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
			throw Mer::Error(tok->to_string() +  "-convert failed(Token can't convert to Id).");
		}
		static std::deque<std::map<std::string, Id*>> &id_table()
		{
			static std::deque<std::map<std::string, Id*>> ret(1);
			return ret;
		}
		static Id* find(std::string str)
		{
			for (size_t i = 0; i < id_table().size(); i++)
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
		static void print()
		{
			int index = 0;
			for (const auto &a : id_table())
			{
				std::cout << "No " << index << " ";
				for (const auto &b : a)
				{
					std::cout << b.first << "  ";
				}
				std::cout << std::endl;
				index++;
			}
			std::cout << "------------------------------------\n";
		}
	private:
		std::string id_name;
	};
	class Integer :public Token
	{
	public:
		Integer(int n) :Token(INTEGER), 
			value(n) {}
		static int get_value(Token *tok)
		{
			if (tok->check(INTEGER))
				return static_cast<Integer*>(tok)->value;
			throw Mer::Error("type-convert failed(Token can't convert to Integer).");
		}
		std::string to_string()const override {
			return "<Integer:" + std::to_string(value) + ">";
		}
	private:
		int value;
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
		Token* this_token(size_t sz)
		{
			/*while (content[pos+sz]->get_tag() == Tag::ENDL)
				advance();*/
			return content[pos+sz];
		}
		Token* this_token()
		{
			while (content[pos]->get_tag() == Tag::ENDL)
				advance();
			return content[pos];
		}
		Tag this_tag()
		{
			return this_token()->get_tag();
		}
		Token *next_token()
		{
			if (pos + 1 < content.size())
				return content[pos + 1];
			else
				throw Error("token_stream out of range");
		}
		Token* get_next_token()
		{
			auto tmp = content[pos];
			advance();
			return tmp;
		}
		void back() {
			if (this_token()->get_tag() == Tag::ENDL || this_token()->get_tag() == Tag::EPT)
			{
				dec();
				back();
			}
			dec();
		}
		void dec()
		{
			--pos;
		}
		void add(Token* tok);
		void advance()
		{
			pos++;
			if (pos >= content.size())
			{
				content.push_back(END_TOKEN);
				throw Error("to the end of token_stream");
			}
		}
		void next()
		{
			if (this_token()->get_tag() == Tag::ENDL|| this_token()->get_tag() == Tag::EPT)
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
		void remove_tokens();
		void clear();
	private:
		std::vector<Token*> content;
		size_t pos = 0;
	};
	class Endl :public Token
	{
	public:
		Endl() :Token(Tag::ENDL)
		{
			line_no = ++current_line;
		}
		static size_t get_value(Token* tok);

		static size_t current_line;
		std::string to_string()const override;
		
	private:
		size_t line_no;
	};
	class String :public Token
	{
	public:
		static std::string get_value(Token *tok)
		{
			if (tok->get_tag() != Tag::STRING)
				throw Error(tok->to_string() + " is not a string");
			return static_cast<String*>(tok)->value;
		}
		String(const std::string&str) :Token(Tag::STRING), value(str) {}
		std::string to_string()const override
		{
			return "<str:" + value + ">";
		}
	private:
		std::string value;
	};
	Token* parse_number(const std::string &str, size_t &pos);
	Token* parse_word(const std::string &str, size_t &pos);
	Token* parse_string(const std::string &str, size_t &pos);
	void build_token_stream(const std::string &content);
	extern TokenStream token_stream;
	size_t get_line_no();

	std::string GIC();
}