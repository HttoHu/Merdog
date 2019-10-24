#pragma once
#include "parser_node.hpp"
namespace Mer
{
	extern std::vector<ParserNode*> pre_stmt;
	// to ensure variables which declared outside are assigned to global locations
	bool& global_stmt();

	struct WordRecorder;
	class Expr;
	class Program:public ParserNode
	{
	public:
		Program(Token *id):identify(id) {}
		Mem::Object execute()override;
		std::string to_string()override;
		std::vector<ParserNode*> stmts;
		size_t* pc = new size_t(0);
	private:
		Token *identify;
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
		std::string to_string()override {
			return type_to_string(type);
		}
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
	// to convert type;
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
	namespace Parser
	{
		Program* program();
		ParserNode *statement();
		ParserNode *var_decl();
		size_t get_type();
		WordRecorder *get_current_info();
	}
}
