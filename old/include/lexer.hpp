#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "value.hpp"
#include "error.hpp"
namespace std
{
	template<typename _T>
	std::string to_string(_T &v, std::decay_t<decltype(v.to_string())>* = nullptr)
	{
		return v.to_string();
	}
}
namespace Merdog
{
	enum TokenType
	{
		New, Class, Namespace,
		Or, And, Not,
		Eq, Ne, Ge, Lt, Gt, Le,
		Assign,
		SAdd, SSub, SMul, SDiv,
		Add, Sub, Mul, Div,
		PP, MM,
		TLiteral, TId, Return,
		Var, Ref, Const, Static,
		TIf, TWhile, TFor,
		Begin, End,
		Line,
		LBk, RBk, LMk, RMk,
		EndStmt,
		Member,
		True,False,
	};
	bool is_operator(TokenType t);
	class Token
	{
	public:
		Token(TokenType t) :token_type(t) {}
		virtual ~Token() {}

		static std::map<TokenType, Token*>&		key_token()
		{
			/*
			Eq, Ne, Ge, Lt, Gt,
			Assign,
			SAdd, SSub, SMul, SDiv,
			Add, Sub, Mul, Div,
			*/
			static  std::map<TokenType, Token*> v = {
				{Member,new Token(Member)},
				{New,new Token(New)},{Class,new Token(Class)},
			{Namespace,new Token(Namespace)},{Or,new Token(Or)},
			{And,new Token(And)},{Not,new Token(Not)},{Eq,new Token(Eq)},
			{Ne,new Token(Ne)},{Ge,new Token(Ge)},{Lt,new Token(Lt)},{Assign,new Token(Assign)},
			{SAdd,new Token(SAdd)},{SSub,new Token(SSub)},{SMul,new Token(SMul)},{SDiv,new Token(SDiv)},
			{Add,new Token(Add)},{Sub,new Token(Sub)},{Mul,new Token(Mul)},{Div,new Token(Div)},
			{PP,new Token(PP)},{MM,new Token(MM)},{Return,new Token(Return)},{Var,new Token(Var)},
			{Const,new Token(Const)},{Static,new Token(Static)},{TIf,new Token(TIf)},{TWhile,new Token(TWhile)},
			{TFor,new Token(TFor)},{Begin,new Token(Begin)},{End,new Token(End)},{LBk,new Token(LBk)},{RBk,new Token(RBk)},
			{RMk,new Token(RMk)},{LMk,new Token(LMk)},{EndStmt,new Token(EndStmt)},{Le,new Token(Le)}
			};
			return v;
		}
		static std::map<TokenType, std::string>&show_token_type()
		{
			static std::map<TokenType, std::string>ret=
			{
				{PP,"<++>"},{MM,"<-->"},{New,"new"},{Class,"class"},
				{Member,"<.>"},
				{ Eq,"<==>" },{ Assign,"<=>" },{ Ne,"<!=>" },{ Ge,"< >=>" },{ Lt,"<<>" },
				{ Gt,"<>>" },{ Le,"<<=>" },{ SAdd,"<+=>" },{ SSub,"<-=>" },
				{ SMul,"<*=>" },{ SDiv,"</=>" },{ Add,"<+>" },{ Sub,"<->" },{ Mul,"<*>" },{ Div,"</>" },
				{ Return,"<return>" },{ Var,"<var>" },{ Ref,"<ref>" },{ Const,"<const>" },
				{ Static,"<static>" },{ TIf,"<if>" },{ TWhile,"<while>" },{ TFor,"<for>" },
				{ Begin,"<{>" },{ End,"<}>" },{ LBk,"<(>" },{ RBk,"<)>" },{ LMk,"<[>" },{ RMk,"<]>" },
				
				{ EndStmt,"<;>" }
			};
			return ret;
		}
		static std::map<std::string, Token*> &	symbol_map()
		{
			static std::map<std::string, Token*> ret={
				{".",key_token()[Member]},
				{"+",key_token()[Add]},{"-",key_token()[Sub]},
				{ "*",key_token()[Mul] }, {"/",key_token()[Div]},
				{ "+=",key_token()[SAdd] },{ "-=",key_token()[SSub] },
				{ "*=",key_token()[SMul] },{ "/=",key_token()[SDiv] },
				{ ">",key_token()[Gt] },{ "<",key_token()[Lt] },
				{ ">=",key_token()[Ge] },{ "<=",key_token()[Le] },
				{ "==",key_token()[Eq] },{"!=",key_token()[Ne]},
				{ "=",key_token()[Assign] },{ ";",key_token()[EndStmt] },
				{ "(",key_token()[LBk] },{ ")",key_token()[RBk] },
				{ "[",key_token()[LMk] },{ "]",key_token()[RMk] },
				{ "{",key_token()[Begin] },{ "}",key_token()[End] }
			};
			return ret;
		}
		static std::map<std::string, Token*> &	keyword_map()
		{
			static std::map<std::string, Token*> ret = {
				{ "new",key_token()[New] },
				{ "class",key_token()[Class] },{ "namespace",key_token()[Namespace] },
				{ "var",key_token()[Var] },{ "ref",key_token()[Ref] },
				{ "const",key_token()[Const] },{ "static",key_token()[Static] },
				{ "if",key_token()[TIf] },{ "while",key_token()[TWhile] },
				{ "for",key_token()[TFor] }
			};
			return ret;
		}
		TokenType get_token_type()const {
			return token_type;
		}
		virtual std::string to_string()const
		{
			auto result = show_token_type().find(token_type);
			if (result != show_token_type().end())
				return result->second;
			else
				throw Merdog::Error("no found such tag");
		}
	private:
		TokenType			 token_type;
	};
	class Id :public Token
	{
	public:
		Id(const std::string & str) :Token(TokenType::TId), id_name(str) {}
		static std::vector<std::map<std::string, Id*>> &
			id_table()
		{
			static std::vector<std::map<std::string, Id*>> ret(1);
			return ret;
		}
		static Id*
			find_id(const std::string & str)
		{
			for (int i = id_table().size() - 1; i >= 0; i--)
			{
				auto result = id_table()[i].find(str);
				if (result == id_table()[i].end())
					continue;
				return result->second;
			}
			return nullptr;
		}

		static void
			delete_id_table()
		{
			for (auto & a : id_table())
				for (auto & b : a)
					delete b.second;
			id_table().clear();
		}
		virtual std::string to_string()const override
		{
			return "<id:" + id_name + ">";
		}
		size_t add = 0;
	private:
		static Id*
			push_id(const std::string &id_name)
		{
			auto a = new Id(id_name);
			id_table().front().insert({ id_name ,a });
			return a;
		}
		std::string
			id_name;
	};
	class LineNo :public Token
	{
	public:
		LineNo() :Token(TokenType::Line)
		{
			static size_t currentLineNo = 1;
			line_no = currentLineNo++;
		}

		static void	get_line_no(Token *tok, size_t &line_index)
		{
			if (tok->get_token_type() != Line)
				throw Merdog::Error("unexpected token");
			line_index = static_cast<LineNo*>(tok)->line_no;
		}
		std::string		to_string()const override
		{
			if(line_no==1)
				return "<Line:" + std::to_string(line_no) + ">";
			return "\n<Line:" + std::to_string(line_no) + ">";
		}
	private:
		size_t			line_no;
	};
	class Literal :public Token
	{
	public:
		Literal(const Value &v):Token(TokenType::TLiteral),lv(v) {}
		std::string to_string()const override
		{
			std::string addition = "";
			if (lv.get_type() == Value::LONG)
				addition = "L";
			return "<"+lv.to_string()+addition+">";
		}
	private:
		Value lv;
	};
	typedef std::vector<Token*> TokenStream;
	extern int index;
	void scan();
}
