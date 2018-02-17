#pragma once
#include "expr.hpp"
#include <unordered_map>
namespace Mer
{
	extern std::unordered_map<std::string, Mem::Raw> glo_mem;
	class Block;
	class Compound;
	class VarDecl;
	class Block :public AST
	{
	public:
		Block() {}
		Mem::Raw get_value();
		Compound *compound_list;
	};
	class Program :public AST
	{
	public:
		Program(const std::string &str, Block *blok) :name(str), block(blok) {}
		Mem::Raw get_value()
		{
			return block->get_value();
		}
	private:
		std::string name;
		Block *block;
	};
	class Type :public AST
	{
	public:
		Type(Token *t) :type(t) {}
		Mem::Raw emit_var()
		{
			if (type->get_tag() == INTEGER_DECL)
				return Mem::Raw(new Mem::Int(0));
			else if (type->get_tag() == REAL_DECL)
				return Mem::Raw(new Mem::Double(0));
			else
				throw Error("create var failed");
		}
	private:
		Token *type;
	};
	class VarDecl :public AST
	{
	public:
		Mem::Raw get_value();
		void init_var_list(const std::map<Token*,Expr*> &v);
		std::map<size_t,Expr*> var_list;
		//std::vector<Token*> var_list;
		Type *type;
	};
	class Print:public AST
	{
	public:
		Print(Token *t) :tok(t) {}
		Mem::Raw get_value()override;
	private:
		Token *tok;
	};
	class Compound :public AST
	{
	public:
		Compound() {}
		Mem::Raw get_value()override
		{
			for (auto &a : children)
			{
				a->get_value();
			}
			return nullptr;
		}
		std::vector<AST*> children;
	};
	class Assign :public AST
	{
	public:
		Assign(size_t l, Token* o, AST* r) :left(l), op(o), right(r) {}
		Mem::Raw get_value()override;
	private:
		size_t left;
		Token *op;
		AST* right;
	};
	class Var :public AST
	{
	public:
		Var(Token *t);
		Mem::Raw get_value()override;
		size_t get_pos()
		{
			return pos;
		}
	private:
		size_t pos;
	};
	class NoOp :public AST
	{};
	namespace Parser
	{
		void print_var_list();
		AST *parse();
		Program *program();
		Block *block();
		VarDecl *variable_declaration();
		AST *type_spec();
		AST *compound_statement();
		std::vector<AST*> statemnet_list();
		AST *statement();
		AST *print_statement();
		AST *assignment_statement();
		AST *variable();
		AST *empty();
	}
}