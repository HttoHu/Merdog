/*
		MIT License

		Copyright (c) 2019 HttoHu

		Permission is hereby granted, free of charge, to any person obtaining a copy
		of this software and associated documentation files (the "Software"), to deal
		in the Software without restriction, including without limitation the rights
		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the Software is
		furnished to do so, subject to the following conditions:

		The above copyright notice and this permission notice shall be included in all
		copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
		SOFTWARE.

*/
#pragma once
#include "parser_node.hpp"
#include "lexer.hpp"
namespace Mer
{

	extern std::vector<ParserNode*> structure_parent_stack;

	class Cast;
	// the santence do nothing.
	class NonOp :public ParserNode
	{
	public:
		Mem::Object execute()override
		{
			return nullptr;
		}
		type_code_index get_type()override
		{
			return Mem::BasicType::BVOID;
		}
	private:
	};
	// to run the operator[]of different objs, you can use BinOp to replace it ,but the performance of Index is better.
	class Index :public ParserNode
	{
	public:
		Index(ParserNode* l, size_t _index, type_code_index _type = -1);
		Mem::Object execute()override;
		type_code_index get_type()override;
	private:
		type_code_index type;
		ParserNode* left;
		type_code_index index;
	};
	/*
	to get the elems of an array but
	 eg. arr[p];
	 the p is a var so or an expr;
	*/
	class ContainerIndex :public ParserNode
	{
	public:
		ContainerIndex(type_code_index t,size_t _pos, ParserNode* _expr) :type(t),pos(_pos), expr(_expr) {}
		Mem::Object execute()override;
		type_code_index get_type()override;
	private:
		size_t pos;
		type_code_index type;
		ParserNode* expr;
	};
	class ContainerGloIndex :public ParserNode
	{
	public:
		ContainerGloIndex(size_t _pos, type_code_index _type,ParserNode* _expr) :pos(_pos),type(_type), expr(_expr) {}
		size_t get_pos()override;
		Mem::Object execute()override;
		type_code_index get_type()override
		{
			return type;
		}
	private:
		type_code_index type;
		size_t pos;
		ParserNode* expr;
	};
	class SubScript :public ParserNode
	{
	public:
		SubScript(ParserNode* l, ParserNode* s);
		type_code_index get_type()override { return type; }
		Mem::Object execute()override;
	private:
		type_code_index type=0;
		ParserNode* left;
		ParserNode* subscr;
	};
	class LogicalBinOp :public ParserNode
	{
	public:
		LogicalBinOp(ParserNode* l, Token *tok, ParserNode* r);
		type_code_index get_type() override { return Mem::BOOL; }
		Mem::Object execute()override;
	private:
		// true ->and ,false ->or
		bool ta = true;
		ParserNode* left;
		ParserNode* right;
	};
	class BinOp :public ParserNode
	{
	public:
		BinOp(ParserNode* l, Token* o, ParserNode* r);
		Mem::Object execute()override;
		type_code_index get_type()override;
		std::string to_string()override;
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
		type_code_index get_type()override
		{
			return expr->get_type();
		}
		std::string to_string()override {
			return op->to_string() + expr->to_string();
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
		Expr(type_code_index t = 0);
		Expr(ParserNode* node) :tree(node) {}
		type_code_index get_type()override;
		size_t get_pos() override {
			return tree->get_pos();
		}
		Mem::Object execute()override
		{
			return tree->execute();
		}
		std::string to_string()override { return tree->to_string(); }
		ParserNode* root() { return tree; }
		bool constant()const override
		{
			return false;
		}
		virtual ~Expr() {
		}
		// to undertake a particular operation, make use of tree then set tree as a nullptr, delete Expr.
		bool is_bool = true;
		ParserNode* tree;
		type_code_index  expr_type=0;
		ParserNode* assign();
		ParserNode* and_or();
		ParserNode* expr();
		ParserNode* nexpr();
		ParserNode* term();
		ParserNode* member_visit();
		ParserNode* subscript();
		ParserNode* factor();
	private:

	};
	class Type;
	class EmptyList :public ParserNode
	{
	public:
		EmptyList(type_code_index t, size_t sz);
		Mem::Object execute()override;
		std::vector<ParserNode*>& exprs() { return init_v; }
	private:
		std::vector<ParserNode*> init_v;
		type_code_index type_code;
		size_t size;
	};
	class InitList:public ParserNode
	{
	public:
		InitList(size_t);
		// if sz is -1, the size will get from init_v.size();
		InitList(type_code_index t,size_t sz);
		Mem::Object execute()override;
		std::vector<Mem::Object> get_array();
		std::vector<ParserNode*>& exprs() { return init_v; }
		type_code_index get_type()override
		{
			return type;
		}
		size_t get_ele_count() { return size; }
		virtual ~InitList();
	private:
		std::vector<ParserNode*> init_v;
		type_code_index type;
		size_t size;
	};
	class GetAdd :public ParserNode
	{
	public:
		GetAdd();
		GetAdd(ParserNode* tmp) :id(tmp), type(tmp->get_type()) {}
		type_code_index get_type()override;
		Mem::Object execute()override;
	private:
		ParserNode* id;
		type_code_index type;
	};
	class RmRef :public ParserNode
	{
	public:
		RmRef();
		type_code_index get_type()override;
		Mem::Object execute()override;
	private:
		ParserNode* id;
		type_code_index type;
	};
	class NewExpr:public ParserNode
	{
	public:
		NewExpr();
		type_code_index get_type()override {
			return expr->get_type() + 1;
		}
		Mem::Object execute()override;
	private:
		ParserNode* expr;
		
	};
	// !!! delete has been discarded
	class ImplicitConvertion:public Expr
	{
	public:
		ImplicitConvertion(type_code_index _type);
		Mem::Object execute()override;
	private:
		type_code_index type;
	};
}