#pragma once
#include <iostream>
#include <vector>
#include <functional>
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
		virtual Mem::Object run(std::vector<Mem::Object>& objs) { return nullptr; }
		virtual size_t get_type() { return 0; }		
		void set_index(size_t pos);
		bool is_completed;
		int index;
	};
	class Function:public FunctionBase
	{
	public:
		Function(size_t t,Param *p, Block *bl=nullptr):type(t),param(p),blo(bl) {}
		void reset_block(Block *b);
		Param *param;
		Mem::Object run(std::vector<Mem::Object> &objs)override;
		size_t get_type()override { return type; }
		
	private:

		size_t type;
		Block *blo;
	};
	class SystemFunction :public FunctionBase 
	{
	public:
		SystemFunction(size_t t, const std::function<Mem::Object(std::vector<Mem::Object>&)> &fun) :type(t), func(fun) {}
		Mem::Object run(std::vector<Mem::Object> &objs)override
		{
			return func(objs);
		}
		size_t get_type()override
		{
			return type;
		}
	private:
		size_t type;
		std::function<Mem::Object(std::vector<Mem::Object>&)>func;
	};
	namespace Parser
	{
		Param *build_param();
		void build_function();
	}
	extern size_t this_func_type;
	extern std::map<std::string, Function*> function_table;
}