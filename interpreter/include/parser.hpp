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
		bool is_auto_array() { return auto_array; }
		bool is_pointer() { return pointer; }
		Token* get_id() { return id; }
	private:
		Token* id;
		// int a[]={1,2,3};
		bool auto_array=false;
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
		size_t& get_size() { return size; }
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
		void process_unit(VarDeclUnit* a, size_t c_pos);
		size_t pos;
		size_t sum=0;
		std::vector<ParserNode*> exprs;
		size_t type;
	};
	class GloVarDecl :public ParserNode
	{
	public:
		GloVarDecl(std::vector<VarDeclUnit*>& vec, size_t t);
		Mem::Object execute()override;
	private:
		void process_unit(VarDeclUnit* a, size_t c_pos);
		size_t pos=0;
		size_t sum=0;
		std::vector<ParserNode*> exprs;
		size_t type;
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
	class MakeDefault :public ParserNode
	{
	public:
		MakeDefault(size_t ty);
		Mem::Object execute()override;
		size_t get_type()override { return type; }
	private:
		Mem::Object ret;
		size_t type;
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
		size_t get_type()override
		{
			return Mem::INT;
		}
	private:
		Mem::Object obj;
	};
	namespace Parser
	{
		Program* program();
		ParserNode *statement();
		ParserNode *var_decl();
		size_t get_type();
		ParserNode* make_var();
		WordRecorder *get_current_info();
	}
}
