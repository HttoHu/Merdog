#pragma once
#include "expr.hpp"
#include "block.hpp"
namespace Mer
{
	class WordRecorder;
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
		Return(Expr* e) :expr(e) {}
		Mem::Object execute()override { throw this; }
		Expr* get_expr() { return expr; }
	private:
		Expr *expr;
	};
	namespace Parser
	{
		Program* program();
		Block* block();
		Block* pure_block();
		ParserNode *statement();
		Assign *assignment_statement();
		VarDecl *var_decl();
		size_t get_type();

		WordRecorder *get_current_info();
	}
}