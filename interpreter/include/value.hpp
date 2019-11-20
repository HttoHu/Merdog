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
#include <vector>
#include "memory.hpp"
#include "parser_node.hpp"
#include "lexer.hpp"
#include "../include/parser.hpp"
namespace Mer
{
	class Namespace;
	class Expr;
	class FunctionBase;
	class StructureBase;

	//literal-const

	class LConV :public ParserNode
	{
	public:
		LConV(Token *t);
		LConV(Mem::Object _obj, size_t t) :obj(_obj), type(t) {}
		size_t get_type()override
		{
			return type;
		}
		Mem::Object execute()override
		{
			return obj;
		}
		std::string to_string()override
		{
			return obj->to_string();//tok->to_string();
		}
		bool constant()const override
		{
			return true;
		}
	private:
		size_t type;
		Mem::Object obj;
	};

	class Namespace;

	class GVar :public ParserNode
	{
	public:
		GVar(size_t _type, size_t off_pos) :type(_type), pos(off_pos) {}
		GVar(WordRecorder *result);
		GVar(WordRecorder* result, size_t offset);
		size_t get_type()override
		{
			return type;
		}
		size_t get_pos()override
		{
			return pos;
		}
		Mem::Object execute()override
		{
			return mem[pos];
		}
		bool constant()const override
		{
			return false;
		}
	private:
		size_t type;
		size_t pos;
	};
	class Variable :public ParserNode
	{
	public:
		Variable(WordRecorder* wr);
		Variable(size_t _type, size_t _pos) :type(_type), pos(_pos) {}
		size_t get_type()override;
		size_t get_pos()override;
		std::string to_string()override
		{
			return "(" + type_to_string(type) + ")" + std::to_string(pos);
		}
		Mem::Object execute()override;
		bool& arr() { return is_arr; }
	private:
		bool is_arr=false;
		size_t type;
		size_t pos;
	};

	class FunctionCall :public ParserNode
	{
	public:
		FunctionCall(FunctionBase * fun, const std::vector<ParserNode*>& exprs);
		size_t get_type()override;
		Mem::Object execute()override;
		std::string to_string()override;
	private:
		FunctionBase * func;
		size_t off_set=0;
		std::vector<ParserNode*> argument;
	};
	class MemberFunctionCall :public ParserNode
	{
	public:
		MemberFunctionCall(FunctionBase* fun, std::vector<ParserNode*>& exprs,ParserNode* _p);
		size_t get_type()override;
		Mem::Object execute()override;
		std::string to_string()override;
	private:
		size_t off=0;
		FunctionBase* func;
		std::vector<ParserNode*> argument;
		ParserNode* parent;
		std::vector<Mem::Object> obj_vec;
	};
	/*
		the initializer of a class
	*/
	struct WordRecorder;
	namespace Parser
	{

		ParserNode* parse_glo(WordRecorder* var_info);
		ParserNode *parse_id();
		ParserNode *parse_var(WordRecorder* var_info);
		// parse id with namespace.
		ParserNode *_parse_id_wn(Namespace *names);
		FunctionCall* parse_initializer(size_t type);
		std::vector<ParserNode*> parse_arguments();
		ParserNode *parse_function_call(Namespace *names);
		MemberFunctionCall* parse_call_by_function(FunctionBase*);
		Namespace *kill_namespaces();
	}
}