#pragma once
#include "expr.hpp"
#include "block.hpp"
namespace Mer
{
	extern std::vector<ParserNode*> pre_stmt;
	// to ensure variables which declared outside are assigned to global locations
	extern bool global_stmt;

	struct WordRecorder;
	
	class Program:public ParserNode
	{
	public:
		Program(Token *id,Block* b):identify(id),blo(b) {}
		Mem::Object execute()override
		{
			for (auto a : pre_stmt)
			{
				a->execute();
			}
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
	class NamePart
	{
	public:
		NamePart();
		size_t get_count() { return count; }
		bool is_array() { return arr; }
		Token* get_id() { return id; }
	private:
		Token* id;
		bool arr = false;
		size_t count=0;
	};
	class VarDeclUnit
	{
	public:
		VarDeclUnit(size_t t);
		ParserNode* get_expr() { return expr; }
		Token* get_id() { return id; }
	private:
		size_t type_code;
		Token* id;
		ParserNode* expr;
	};
	class LocalVarDecl :public ParserNode
	{
	public:
		LocalVarDecl(std::vector<VarDeclUnit*>& vec,size_t t);
		Mem::Object execute()override;
	private:
		size_t pos;
		std::vector<VarDeclUnit*> units;
		size_t type;
	};
	class GloVarDecl :public ParserNode
	{
	public:
		GloVarDecl(std::vector<VarDeclUnit*>& vec, size_t t);
		Mem::Object execute()override;
	private:
		std::vector<VarDeclUnit*> units;
		size_t pos;
		size_t type;
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
	namespace Parser
	{
		Program* program();
		Block* block();
		Block* pure_block();
		ParserNode *statement();
		ParserNode *var_decl();
		size_t get_type();
		WordRecorder *get_current_info();
	}
}
