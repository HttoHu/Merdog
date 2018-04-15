#pragma once
#include <iostream>
#include <vector>
#include "expr.hpp"
namespace Mer
{
	class Block;
	class Param
	{
	public:
		bool type_check(const std::vector<size_t> &types);
		void push_new_param(size_t type, size_t pos)
		{
			param_pos.push_back({ type,pos });
		}
		std::vector<std::pair<size_t, size_t>> &get_param_table() { return param_pos; }
	private:
		size_t param_size;
		// type + pos;
		std::vector<std::pair<size_t,size_t>> param_pos;
	};
	class FunctionBase
	{
	public:
		FunctionBase() {}
		virtual Mem::Object run(const std::vector<Expr*> exprs){ }
		virtual size_t get_type(){}
	};
	class Function:public FunctionBase
	{
	public:
		Function(size_t t,Param *p, Block *bl):type(t),param(p),blo(bl) {}
		Mem::Object run(const std::vector<Expr*> exprs)override;
		size_t get_type()override { return type; }
	private:
		size_t type;
		Param *param;
		Block *blo;
	};
	class SystemFunction :public FunctionBase 
	{

	};
	namespace Parser
	{
		Param *build_param();
		void build_function();
	}
	std::map<std::string, Function*> function_table;
}