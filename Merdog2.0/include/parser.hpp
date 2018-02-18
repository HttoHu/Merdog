#pragma once
#include "expr.hpp"
#include <unordered_map>
namespace Mer
{
	class Block;
	class Compound;
	class VarDecl;
	class Block :public AST
	{
	public:
		Block() {}
		Mem::Object get_value();
		Compound *compound_list;
	};
	class Program :public AST
	{
	public:
		Program(const std::string &str, Block *blok) :name(str), block(blok) {}
		Mem::Object get_value()
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
		Type(Token *t) {
			switch (t->get_tag())
			{
			case INTEGER_DECL:
				type_v = Mem::BasicType::INT;
				break;
			case REAL_DECL:
				type_v = Mem::BasicType::DOUBLE;
				break;
			case STRING_DECL:
				type_v = Mem::BasicType::STRING;
				break;
			default:
				break;
			}
		}
		Mem::Object emit_var()
		{
			switch (type_v)
			{
			case Mer::Mem::INT:
				return Mem::Object(new Mem::Int(0));
			case Mer::Mem::DOUBLE:
				return Mem::Object(new Mem::Double(0));
			case Mer::Mem::STRING:
				return Mem::Object(new Mem::String(""));
			default:
				throw Error("create var failed");
			}
		}
		Mem::Object adapt_value(Mem::Object &value)
		{
			return Mem::Object(value->Convert(type_v));
		}
	private:
		Mem::BasicType type_v;
	};
	class VarDecl :public AST
	{
	public:
		Mem::Object get_value();
		void init_var_list(const std::map<Token*,Expr*> &v);
		std::map<size_t,Expr*> var_list;
		//std::vector<Token*> var_list;
		Type *type;
	};
	class Print:public AST
	{
	public:
		Print(Token *t) :tok(t) {}
		Mem::Object get_value()override;
	private:
		Token *tok;
	};
	class Compound :public AST
	{
	public:
		Compound() {}
		Mem::Object get_value()override
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
		Mem::Object get_value()override;
	private:
		size_t left;
		Token *op;
		AST* right;
	};
	class Var :public AST
	{
	public:
		Var(Token *t);
		Mem::Object get_value()override;
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