/*
		MIT License

		Copyright (c) 2019 HttoHu

		Permission is hereby granted, free of charge, to any person obtaining a copy
		of this software and associated documentation files (the "Software"), to deal
		in the Software without restriction, including without limitation the rights
		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the Software is
		furnished to do so, subject to the following conditions:

		The above copyright notice and this permission notice shall be included in all
		copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
		SOFTWARE.

*/
#pragma once
#include <deque>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "error.hpp"
namespace Mer
{
	extern std::string input_buf;
	extern std::stringstream my_stringstream;
	void build_token_stream(const std::string& content);
	//=========================================================================
	enum Tag
	{
		EPT=0,
		SADD, SSUB, SMUL, SDIV, ASSIGN,
		EQ, NE, GT, GE, LT, LE,
		PLUS, MINUS, MUL, DIV,
		IMPORT, NAMESPACE, STRUCT,
		PRINT, CAST,
		GET_ADD,PTRVISIT,
		VOID_DECL, INTEGER_DECL, REAL_DECL, STRING_DECL, BOOL_DECL,CHAR_DECL,
		PROGRAM,
		FUNCTION, RETURN,
		IF, ELSE_IF, ELSE, DO,WHILE, FOR, BREAK, CONTINUE,SWITCH,CASE,
		DEFAULT,SIZEOF,
		NEW, MAKE,
		NOT, AND, OR,
		REF, BEGIN, END, SEMI, DOT, COMMA,
		ID, INTEGER, REAL,CHAR_LIT, COLON,

		TTRUE, TFALSE,
		LPAREN, RPAREN,LSB,RSB,
		ENDOF, ENDL,
		STRING,NULLPTR,
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
	extern std::map<std::string, Token*>BasicToken;
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
			if(tok->get_tag()!=REAL)
				throw Mer::Error("type-convert failed(Token can't convert to Real).");
			return static_cast<Real*>(tok)->value;
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
		Token* next_token();
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
		void advance();
		void next()
		{
			if (this_token()->get_tag() == Tag::ENDL|| this_token()->get_tag() == Tag::EPT)
			{
				advance();
				next();
			}
			advance();
		}
		void match(Tag t);
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
		friend void Mer::build_token_stream(const std::string& content);
		std::vector<Token*> _rem_tok_vec;
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
	class CharToken :public Token
	{
	public:
		static char get_value(Token* t) {
			return static_cast<CharToken*>(t)->ch;
		}
		CharToken(char c) :Token(Tag::CHAR_LIT), ch(c) {}
		std::string to_string()const override
		{
			return "<char: " + std::string(1, ch) + ">";
		}
	private:
		char ch;
	};
	void preprocess(const std::string &str,size_t & pos);
	//=======================================================
	Token* parse_number(const std::string& str, size_t& pos);
	Token* parse_word(const std::string& str, size_t& pos);
	Token* parse_string(const std::string& str, size_t& pos);
	extern TokenStream token_stream;
	size_t get_line_no();
	Token* parse_char(const std::string& str, size_t& pos);
	std::string get_this_id_string_value();
}