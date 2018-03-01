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
		void new_block();
		void end_block();
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
			case BOOL_DECL:
				type_v = Mem::BasicType::BOOL;
				break;
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
			case Mer::Mem::BOOL:
				return std::make_shared<Mem::Bool>(false);
			case Mer::Mem::INT:
				return std::make_shared<Mem::Int>(0);
			case Mer::Mem::DOUBLE:
				return std::make_shared<Mem::Double>(0.0);
			case Mer::Mem::STRING:
				return std::make_shared<Mem::String>("");
			default:
				throw Error("create var failed");
			}
		}
		Mem::Object adapt_value(const Mem::Object &value)
		{
			return Mem::Object(value->Convert(type_v));
		}
	private:
		Mem::BasicType type_v;
	};
	class CallFunc :public AST
	{
	public:
		CallFunc(Token *fun, std::vector<Expr*> &vec);
		Mem::Object get_value()override;
	private:
		size_t rvs;
		size_t func;
		std::vector<Expr*> args;
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
	class RefDecl :public AST
	{
	public:
		Mem::Object get_value();
		void init_var_list(const std::map<Token*, Token*> &v);
		std::map<size_t, Token*> var_list;
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
		std::deque<AST*> children;
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
	class AssignPlus :public AST
	{
	public:
		AssignPlus(size_t l, Token* o, AST* r) :left(l), op(o), right(r) {}
		Mem::Object get_value()override;
	private:
		size_t left;
		Token *op;
		AST* right;
	};
	class AssignMinus :public AST
	{
	public:
		AssignMinus(size_t l, Token* o, AST* r) :left(l), op(o), right(r) {}
		Mem::Object get_value()override;
	private:
		size_t left;
		Token *op;
		AST* right;
	};
	class AssignMul :public AST
	{
	public:
		AssignMul(size_t l, Token* o, AST* r) :left(l), op(o), right(r) {}
		Mem::Object get_value()override;
	private:
		size_t left;
		Token *op;
		AST* right;
	};
	class AssignDiv :public AST
	{
	public:
		AssignDiv(size_t l, Token* o, AST* r) :left(l), op(o), right(r) {}
		Mem::Object get_value()override;
	private:
		size_t left;
		Token *op;
		AST* right;
	};
	class Return :public AST
	{
	public:
		Return(Expr* expr) :ret_value(expr) {}
		Mem::Object get_value()override {
			throw this;
		}
		Mem::Object get_ret_value()
		{
			auto ret= ret_value->get_value();
			if (ret == nullptr)
				std::cout << "FUCKER";
			return ret;
		}
	private:
		Expr * ret_value;
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
		RefDecl *ref_declaration();
		VarDecl *variable_declaration();
		Type *type_spec();
		AST *compound_statement();
		std::vector<AST*> statemnet_list();
		CallFunc *func_call();
		AST *statement();
		AST *id_event();
		AST *print_statement();
		AST *assignment_statement();
		AST *variable();
		AST *empty();
	}
}