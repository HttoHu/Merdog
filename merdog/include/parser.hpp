#pragma once
#include "expr.hpp"
#include "block.hpp"
namespace Mer
{
	struct WordRecorder;
	class Program:public ParserNode
	{
	public:
		Program(Token *id,Block* b):identify(id),blo(b) {}
		Mem::Object execute()override
		{
			blo->execute();
			return nullptr;
		}
		std::string to_string()override
		{
			return identify->to_string();
		}
	private:
		Token *identify;
		Block *blo;
	};
	class VarDeclUnit :public ParserNode
	{
	public:
		VarDeclUnit(size_t t, Token *tok, Expr* _expr);
		Mem::Object execute()override;
	private:
		size_t type;
		size_t pos;
		Expr* expr;
	};
	class VarDecl :public ParserNode
	{
	public:
		VarDecl(size_t t,const std::map<Token*, Expr*> &v);
		Mem::Object execute()override;
	private:
		size_t type;
		std::vector<std::pair<std::size_t, Expr *>> var_list;
	};
	class Print :public ParserNode
	{
	public:
		Print(Token *tok);
		Mem::Object execute()override;
	private:
		size_t pos;
		Token * content;
	};
	class Return :public ParserNode
	{
	public:
		Return(Expr* e,Block *blo) :expr(e), block(blo) 
		{
			if (block == nullptr)
				throw Error("What hell it was");
		}
		Mem::Object execute()override;
		Expr* get_expr() { return expr; }
	private:
		Expr *expr;
		Block *block;
	};
	class ArrayDecl :public ParserNode
	{
	public:
		ArrayDecl(size_t t, size_t sz, Token *tok, InitList* _expr);
		Mem::Object execute()override;
	private:
		size_t type;
		size_t size;
		size_t pos;
		InitList* expr;
	};
	namespace Parser
	{
		Program* program();
		Block* block();
		Block* pure_block();
		ParserNode *statement();
		ParserNode *var_decl();
		size_t get_type();
		ArrayDecl* array_decl(size_t type_code);
		WordRecorder *get_current_info();
	}
}
