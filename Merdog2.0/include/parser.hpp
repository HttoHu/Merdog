#pragma once
#include "expr.hpp"
#include <unordered_map>
namespace Mer
{
	extern std::unordered_map<std::string, Value> glo_mem;
	class Compound :public AST
	{
	public:
		Compound() {}
		Value get_value()override
		{
			for (auto &a : children)
				a->get_value();
			return 0;
		}
		std::vector<AST*> children;
	};
	class Assign :public AST
	{
	public:
		Assign(AST* l,Token* o, AST* r) :left(l), op(o),right(r) {}
		Value get_value()override;
	private:
		AST* left;
		Token *op;
		AST* right;
	};
	class Var :public AST
	{
	public:
		Var(Token *t) :tok(t) {
		}
		static std::string get_name(Var *v)
		{
			return Id::get_value(v->tok);
		}
		Value get_value()override 
		{
			auto result = glo_mem.find(get_name(this));
			if (result == glo_mem.end())
				throw Error("var \'"+get_name(this)+"\' no found");
			else
				return result->second;
		}
	private:
		Token *tok;
	};
	class NoOp :public AST
	{};
	namespace Parser
	{
		void print_var_list();
		AST *parse();
		AST *program();
		AST *compound_statement();
		std::vector<AST*> statemnet_list();
		AST *statement();
		AST *assignment_statement();
		AST *variable();
		AST *empty();
	}
}