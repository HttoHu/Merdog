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
#include "lexer.hpp"
#include "parser_node.hpp"
namespace Mer
{
	extern std::vector<UptrPNode> pre_stmt;
	// to ensure variables which declared outside are assigned to global locations
	bool& global_stmt();

	struct WordRecorder;
	class Expr;
	class Program :public ParserNode
	{
	public:
		Program(Token* id) :identify(id) {}
		Mem::Object execute()override;
		std::string to_string()override;
		std::vector<UptrPNode> stmts;
		size_t* pc = new size_t(0);
		~Program() { delete pc; }
		size_t off = 0;
		Token* identify;
	};
	class NamePart
	{
	public:
		NamePart();
		size_t get_count() { return count; }
		bool is_array() { return arr; }
		bool is_auto_array() { return auto_array; }
		bool is_pointer() { return pointer; }
		Token* get_id() { return id; }
		std::vector<size_t> array_indexs;
	private:
		Token* id;
		// int a[]={1,2,3};
		bool auto_array = false;
		bool arr = false;
		bool pointer = false;
		size_t count = 1;
	};
	class VarDeclUnit
	{
	public:
		VarDeclUnit(type_code_index t);
		ParserNode* get_expr() { return expr; }
		Token* get_id() { return id; }
		size_t& get_size() { return size; }
		bool arr() { return is_arr; }
		bool pointer() { return is_p; }		
		std::vector<size_t> array_indexs;
	private:

		bool is_arr = false;
		bool is_p = false;
		size_t size = 1;
		type_code_index type_code;
		Token* id;
		ParserNode* expr;
	};
	//create local variable
	class LocalVarDecl :public ParserNode
	{
	public:
		LocalVarDecl(std::vector<VarDeclUnit*>& vec, type_code_index t);
		Mem::Object execute()override;
		std::string to_string()override {
			return type_to_string(type);
		}
	private:
		void process_unit(VarDeclUnit* a, size_t c_pos);
		type_code_index pos;
		type_code_index sum = 0;
		std::vector<UptrPNode> exprs;
		type_code_index type;
	};
	//create global variable
	class GloVarDecl :public ParserNode
	{
	public:
		GloVarDecl(std::vector<VarDeclUnit*>& vec, type_code_index t);
		Mem::Object execute()override;
	private:
		void process_unit(VarDeclUnit* a, size_t c_pos);
		type_code_index pos = 0;
		int sum = 0;
		std::vector<UptrPNode> exprs;
		type_code_index type;
	};
	// to convert type;
	class Cast :public ParserNode
	{
	public:
		Cast();
		Cast(ParserNode* _expr, type_code_index type) :expr(_expr), to_type(type) {}
		Mem::Object execute()override;
		type_code_index get_type()override { return to_type; }
		~Cast() { delete expr; }
	private:
		ParserNode* expr;
		type_code_index to_type;
	};
	class MakeDefault :public ParserNode
	{
	public:
		MakeDefault(type_code_index ty);
		Mem::Object execute()override;
		type_code_index get_type()override { return type; }
	private:
		Mem::Object ret;
		type_code_index type;
	};
	// get the element count of array
	class SizeOf :public ParserNode
	{
	public:
		SizeOf();
		Mem::Object execute()override
		{
			return obj;
		}
		type_code_index get_type()override
		{
			return Mem::INT;
		}
	private:
		Mem::Object obj;
	};
	struct ArrayInitList {
		ArrayInitList(const std::vector<ParserNode*> _leaves);
		ArrayInitList(const std::vector<ArrayInitList*> _children);
		std::vector<ArrayInitList*> children;
		std::vector<ParserNode*> leaves;
		~ArrayInitList() {
			for (auto a : children)
				delete a;
		}
		bool leaves_parent() { return leaves.size() != 0; }
		size_t size() { if (leaves_parent())return leaves.size(); return children.size(); }
	};
	namespace Parser
	{
		std::pair<std::vector<size_t> ,std::vector<ParserNode*>> linearized_array();
		// DFS 
		ArrayInitList* build_array_initlist_tree();
		std::unique_ptr<Program> program();
		ParserNode* statement();
		ParserNode* var_decl();
		type_code_index get_type();
		ParserNode* make_var();
		WordRecorder* get_current_info();
	}
}
