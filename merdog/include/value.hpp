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
	private:
		size_t type;
		size_t pos;
	};

	class FunctionCall :public ParserNode
	{
	public:
		FunctionCall(const std::vector<size_t> &types, size_t _index, FunctionBase * fun, std::vector<ParserNode*>& exprs);
		size_t get_type()override;
		Mem::Object execute()override;
		std::string to_string()override;
	private:
		size_t index;
		FunctionBase * func;
		std::vector<ParserNode*> argument;
	};

	struct WordRecorder;

	namespace Parser
	{

		ParserNode* parse_glo(WordRecorder* var_info);
		ParserNode *parse_array(WordRecorder* var_info);
		ParserNode *parse_id();
		ParserNode *parse_var(WordRecorder* var_info);
		// parse id with namespace.
		ParserNode *_parse_id_wn(Namespace *names);
		//FunctionCall *parse_function_call(Mer::Expr *co_caller,StructureBase *sb);
		FunctionCall *parse_function_call(Namespace *names);
		FunctionCall* parse_call_by_function(FunctionBase*,ParserNode *parent);
		Namespace *kill_namespaces();
	}
}