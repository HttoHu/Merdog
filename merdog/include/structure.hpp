#pragma once
#include <vector>
#include <map>
#include <set>
#include "parser.hpp"
namespace Mer
{
	enum MemberType
	{
		FUNC = 0, VAR = 1
	};
	class FunctionBase;
	// to init build_in type such as vector..
	class StructureBase
	{
	public: 
		virtual FunctionBase* get_function(const std::string &id) { return nullptr; };
		virtual ~StructureBase() {}
	private:
	};
	class SComVarFuncCall:public ParserNode
	{
	public:
		SComVarFuncCall(const std::vector<size_t> &types, FunctionBase * fun, std::vector<Expr*>& exprs);
		size_t get_type()override;
		Mem::Object execute()override;
	private:
		Mem::Object obj;
		FunctionBase * func;
		std::vector<Expr*> argument;
	};
	namespace Parser
	{
		ParserNode* parse_compound_var_id();
	}
}