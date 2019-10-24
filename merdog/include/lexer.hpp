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
namespace mer
{
	enum Tag
	{
		EPT=0,
		IMPORT, NAMESPACE, STRUCT,
		PRINT, CAST,
		SADD, SSUB, SMUL, SDIV, GET_ADD, PTRVISIT,
		VOID_DECL, INTEGER_DECL, REAL_DECL, STRING_DECL, BOOL_DECL,CHAR_DECL,
		PROGRAM,
		GE, LE, GT, LT, NE, EQ,
		FUNCTION, RETURN,
		IF, ELSE_IF, ELSE, DO, WHILE, FOR, BREAK, CONTINUE, SWITCH, CASE,
		DEFAULT,
		NEW, DELETE,
		NOT, AND, OR,
		REF, BEGIN, END, SEMI, DOT, COMMA,
		ID, INTEGER, REAL, COLON,CHAR,
		PLUS, MINUS, MUL, DIV, ASSIGN,
		TTRUE, TFALSE,
		LPAREN, RPAREN, LSB, RSB,
		ENDOF, ENDL,
		STRING, NULLPTR,
	};
	using index_type = int;
	extern std::map<Tag, std::string> TagStr;
	extern std::map<char, char> escape_character_table;
	std::string tag_to_sign(Tag t);
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
		Id(const std::string& str) :Token(ID), id_name(str)
		{
			id_table().front().insert({ id_name ,this });
		}
		static std::string get_value(Token* tok)
		{
			if (tok->check(ID))
				return static_cast<Id*>(tok)->id_name;
			throw mer::Error(tok->to_string() + "-convert failed(Token can't convert to Id).");
		}
		static std::deque<std::map<std::string, Id*>>& id_table()
		{
			static std::deque<std::map<std::string, Id*>> ret(1);
			return ret;
		}
		static Id* find(std::string str)
		{
			for (index_type i = 0; i < id_table().size(); i++)
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
			for (const auto& a : id_table())
			{
				std::cout << "No " << index << " ";
				for (const auto& b : a)
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
		static int get_value(Token* tok)
		{
			if (tok->check(INTEGER))
				return static_cast<Integer*>(tok)->value;
			throw mer::Error("type-convert failed(Token can't convert to Integer).");
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
		static double get_value(Token* tok)
		{
			return static_cast<Real*>(tok)->value;
			throw mer::Error("type-convert failed(Token can't convert to Real).");
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
		void push_back(mer::Token* tok)
		{
			content.push_back(tok);
		}
		void pop_back()
		{
			content.pop_back();
		}
		void back_to(index_type index);
		index_type current_index();
		Token* this_token(index_type sz)
		{
			/*while (content[pos+sz]->get_tag() == Tag::ENDL)
				advance();*/
			return content[pos + sz];
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
		void back();

		void add(Token* tok);
		void advance();
		void next();
		void match(Tag t);
		void print()
		{
			for (const auto& a : content)
				std::cout << a->to_string();
		}
		std::vector<Token*>& _get_content()
		{
			return content;
		}
		index_type _get_pos()
		{
			return pos;
		}
		void remove_tokens();
		void clear();
	private:
		void dec()
		{
			--pos;
		}
		std::vector<Token*> content;
		index_type pos = 0;
	};
	class Endl :public Token
	{
	public:
		Endl() :Token(Tag::ENDL)
		{
			line_no = ++current_line;
		}
		static int get_value(Token* tok);

		static index_type current_line;
		std::string to_string()const override;

	private:
		index_type line_no;
	};
	class String :public Token
	{
	public:
		static std::string get_value(Token* tok)
		{
			if (tok->get_tag() != Tag::STRING)
				throw Error(tok->to_string() + " is not a string");
			return static_cast<String*>(tok)->value;
		}
		String(const std::string& str) :Token(Tag::STRING), value(str) {}
		std::string to_string()const override
		{
			return "<str:" + value + ">";
		}
	private:
		std::string value;
	};
	class Char :public Token
	{
	public:
		static char get_value(Token* tok);
		Char (char c):Token(Tag::CHAR),ch(c){}
		std::string to_string()const override
		{
			return "<char: " + std::string(1,ch) + ">";
		}
	private:
		char ch;
	};
	Token* parse_char(const std::string& str, int& pos);
	Token* parse_number(const std::string& str, int& pos);
	Token* parse_word(const std::string& str, int& pos);
	Token* parse_string(const std::string& str, int & pos);
	void build_token_stream(const std::string& content);
	extern TokenStream token_stream;
	int get_line_no();

	std::string GIC();
}