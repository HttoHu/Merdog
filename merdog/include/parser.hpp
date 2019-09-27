#pragma once
#include "block.hpp"
namespace Mer
{
	extern std::vector<ParserNode*> pre_stmt;
	// to ensure variables which declared outside are assigned to global locations
	extern bool global_stmt;

	struct WordRecorder;
	class Expr;
	class Program:public ParserNode
	{
	public:
		Program(Token *id,Block* b):identify(id),blo(b) {}
		Mem::Object execute()override;
		std::string to_string()override;
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
		bool is_pointer() { return pointer; }
		Token* get_id() { return id; }
	private:
		Token* id;
		bool arr = false;
		bool pointer = false;
		size_t count=1;
	};
	class VarDeclUnit
	{
	public:
		VarDeclUnit(size_t t);
		ParserNode* get_expr() { return expr; }
		Token* get_id() { return id; }
		size_t get_size() { return size; }
		bool arr() { return is_arr; }
		bool pointer() { return is_p; }
	private:
		bool is_arr = false;
		bool is_p = false;
		size_t size=1;
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
		size_t sum;
		std::vector<Expr*> exprs;
		size_t type;
	};
	class GloVarDecl :public ParserNode
	{
	public:
		GloVarDecl(std::vector<VarDeclUnit*>& vec, size_t t);
		Mem::Object execute()override;
	private:
		size_t pos=0;
		size_t sum;
		std::vector<Expr*> exprs;
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
	class Cast :public ParserNode
	{
	public:
		Cast();
		Cast(ParserNode* _expr, size_t type) :expr(_expr), to_type(type) {}
		Mem::Object execute()override;
		size_t get_type()override { return to_type; }
	private:
		ParserNode* expr;
		size_t to_type;
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
