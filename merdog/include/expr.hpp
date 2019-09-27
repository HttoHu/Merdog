#pragma once
#include "parser_node.hpp"
#include "lexer.hpp"
namespace Mer
{
	class Cast;
	class NonOp :public ParserNode
	{
	public:
		Mem::Object execute()override
		{
			return nullptr;
		}
		size_t get_type()override
		{
			return Mem::BasicType::BVOID;
		}
	private:
	};
	class Index :public ParserNode
	{
	public:
		Index(ParserNode* l, size_t _index) :left(l), index(_index) {}
		Mem::Object execute()override
		{
			return left->execute()->operator[](std::make_shared<Mem::Int>(index));
		}
		size_t get_type()override
		{
			return left->get_type();
		}
	private:
		ParserNode* left;
		size_t index;
	};
	class ContainerIndex :public ParserNode
	{
	public:
		ContainerIndex(size_t t,size_t _pos, ParserNode* _expr) :type(t),pos(_pos), expr(_expr) {}
		Mem::Object execute()override;
		size_t get_type()override;
	private:
		size_t pos;
		size_t type;
		ParserNode* expr;
	};
	class ContainerGloIndex :public ParserNode
	{
	public:
		ContainerGloIndex(size_t _pos, ParserNode* _expr) :pos(_pos), expr(_expr) {}
		size_t get_pos()override;
		Mem::Object execute()override;
	private:
		size_t pos;
		ParserNode* expr;
	};
	class BinOp :public ParserNode
	{
	public:
		BinOp(ParserNode* l, Token* o, ParserNode* r);
		Mem::Object execute()override;
		size_t get_type()override;
		virtual ~BinOp()
		{
			delete left;
			delete right;
		}
	private:
		ParserNode *left;
		Token *op;
		ParserNode *right;
	};
	class UnaryOp :public ParserNode
	{
	public:
		UnaryOp(Token *t, ParserNode* e) :op(t), expr(e) {}
		Mem::Object execute()override;
		size_t get_type()override
		{
			return expr->get_type();
		}
		virtual ~UnaryOp()
		{
			delete expr;
		}
	private:
		Token * op;
		ParserNode* expr;
	};
	class Structure;
	class CompoundObject;
	class Expr :public ParserNode
	{
	public:
		Expr(size_t t = 0);
		Expr(ParserNode* node) :tree(node) {}
		size_t get_type()override;
		size_t get_pos() override {
			return tree->get_pos();
		}
		Mem::Object execute()override
		{
			return tree->execute();
		}
		ParserNode* root() { return tree; }
		bool constant()const override
		{
			return false;
		}
		virtual ~Expr() {
			if (tree != nullptr)
				delete tree;
		}
		// to undertake a particular operation, make use of tree then set tree as a nullptr, delete Expr.
		bool is_bool = true;
		ParserNode* tree;
		size_t expr_type;
		ParserNode* and_or();
		ParserNode* expr();
		ParserNode* nexpr();
		ParserNode* term();
		ParserNode* factor();
	private:


	};
	class Type;
	class EmptyList :public ParserNode
	{
	public:
		EmptyList(size_t t, size_t sz);
		Mem::Object execute()override;
		std::vector<Expr*>& exprs() { return init_v; }
	private:
		std::vector<Expr*> init_v;
		size_t type_code;
		size_t size;
	};
	class InitList:public ParserNode
	{
	public:
		InitList(size_t t,size_t sz);
		Mem::Object execute()override;
		std::vector<Mem::Object> get_array();
		std::vector<Expr*>& exprs() { return init_v; }
		size_t get_type()override
		{
			return type;
		}
		virtual ~InitList();
	private:
		std::vector<Expr*> init_v;
		size_t type;
		size_t size;
	};

	class Assign :public ParserNode
	{
	public:
		enum AssignType
		{
			None,
			Add, Sub, Div, Mul,
			Null,
		};
		Assign(AssignType a, ParserNode* l, Token* o, ParserNode* r);
		Mem::Object execute()override;
		virtual ~Assign()
		{
			delete right;
		}
	private:
		AssignType asType;
		ParserNode* left;
		Token *op;
		ParserNode* right;
	};
	class GetAdd :public ParserNode
	{
	public:
		GetAdd();
		size_t get_type()override;
		Mem::Object execute()override;
	private:
		ParserNode* id;
		size_t type;
		size_t pos;
	};
	class RmRef :public ParserNode
	{
	public:
		RmRef();
		size_t get_type()override;
		Mem::Object execute()override;
	private:
		ParserNode* id;
		size_t type;
	};
	class NewExpr:public ParserNode
	{
	public:
		NewExpr();
		size_t get_type()override {
			return expr->get_type() + 1;
		}
		Mem::Object execute()override;
	private:
		ParserNode* expr;
		
	};
	class Delete :public ParserNode
	{
	public:
		Delete();
		size_t get_type()override {
			return expr->get_type() + 1;
		}
		Mem::Object execute()override;
	private:
		ParserNode* expr;
	};
	class ImplicitConvertion:public Expr
	{
	public:
		ImplicitConvertion(size_t _type);
		Mem::Object execute()override;
	private:
		size_t type;
	};
}