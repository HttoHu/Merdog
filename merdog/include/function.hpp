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
			arg_pos.push_back({ type,pos });
		}
		std::vector<std::pair<size_t, size_t>> &get_param_table() { return arg_pos; }
	private:
		size_t param_size;
		// type + pos;
		std::vector<std::pair<size_t,size_t>> arg_pos;
	};
	class FunctionBase
	{
	public:
		FunctionBase();
		virtual bool check_param(const std::vector<size_t>& types);
		void set_param_types(const std::vector<size_t>& types)
		{
			param_types = types;
		}
		virtual size_t get_type() { return 0; }

		virtual Mem::Object run(std::vector<Mem::Object>& objs) { return nullptr; }
		void set_index(size_t pos);

		bool is_completed;
		int index;
	protected:
		std::vector<size_t> param_types;
	};
	class Function : public FunctionBase
	{
	public:
		Function(size_t t, Param *p, Block *bl = nullptr) ;
		Function(size_t t, Block *bl = nullptr);
		void reset_block(Block *b);
		void reser_param(Param *p);
		Param *param;
		Mem::Object run(std::vector<Mem::Object> &objs)override;
		Block *get_block() { return blo; }
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
		bool check_param(const std::vector<size_t>& types)override;
		void dnt_check_param()
		{
			check_param_type = false;
		}
	private:
		size_t type;
		bool check_param_type = true;
		std::function<Mem::Object(std::vector<Mem::Object>&)>func;
	};
	namespace Parser
	{
		Param *build_param();
		void build_function();
	}
	extern Block *current_function_block;
	extern size_t this_func_type;
	extern std::map<std::string, Function*> function_table;
}