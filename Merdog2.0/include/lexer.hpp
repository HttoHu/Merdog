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
        ID,INTEGER,
		PLUS,MINUS,MUL,DIV,
		LPAREN,RPAREN,
		END,ENDL,
    };
    extern std::map<Tag,std::string> TagStr;
    class Token
    {
        public:
            Token(Tag t):token_type(t){}
            Tag get_tag()const {return token_type;}
            
            bool check(Tag t){
                return token_type==t;
            }
            virtual std::string to_string()const{
                return "<"+TagStr[token_type]+">";
            }
            virtual ~Token(){}
        private:
            Tag token_type;
    };
    extern Token* END_TOKEN;
   // extern std::map<std::string,Token*> key_map;
    class Id final:public Token
    {
        public:
            Id(const std::string &str):Token(ID),id_name(str)
            {
                id_table().front().insert({ id_name ,this });
            }
            static std::string get_value(Token *tok)
            {
                if(tok->check(ID))
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
            std::string to_string()const override{
                return "<Id:"+id_name+">";
            }
        private:
            std::string id_name;
    };
    class Integer:public Token
    {
        public:
            Integer(int64_t n):Token(INTEGER),value(n){}
            static int get_value(Token *tok)
            {
                if(tok->check(INTEGER))
                    return static_cast<Integer*>(tok)->value;
                throw Mer::Error("type-convert failed(Token can't convert to Id).");
            }
            std::string to_string()const override{
                return "<Integer:"+std::to_string(value)+">";
            }
        private:
            int64_t value;
    };
    class TokenStream
    {
        public:
            TokenStream()=default;
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
                return content[pos];
            }
            Token* get_next_token()
            {
                auto tmp=content[pos];
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
            void match(Tag t)
            {
                // to check the token whether it is right, and if matched call advance, or throw an error.
                // example: match(PLUS); 
                if(this_token()->get_tag()==t)
                    advance();
                else
                    throw Error(TagStr[t]+" not matched");
            }
            void print()
            {
                for (const auto &a:content)
                    std::cout<<a->to_string();
            }



            
        private:
            std::vector<Token*> content;
            size_t pos=0;
    };
    Token* parse_interger(const std::string &str,size_t &pos);
    Token* parse_id(const std::string &str,size_t &pos);
    void build_token_stream(const std::string &content);
    extern TokenStream token_stream;
}