#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "expr.hpp"
#include "type.hpp"
namespace Mer
{
	class Block;
	class Param
	{
	public:
		bool type_check(const std::vector<size_t> &types);
		void push_new_param(Mem::Type* type, size_t pos)
		{
			arg_pos.push_back({ type,pos });
		}
		std::vector<std::pair<Mem::Type*, size_t>> &get_param_table() { return arg_pos; }
	private:
		size_t param_size;
		// type + pos;
		std::vector<std::pair<Mem::Type*, size_t>> arg_pos;
	};
	class FunctionBase
	{
	public:
		FunctionBase();
		virtual bool check_param(const std::vector<Mem::Type*>& types);
		virtual void convert_arg(std::vector<Expr*> &args);
		void set_param_types(const std::vector<Mem::Type*>& types)
		{
			param_types = types;
		}
		virtual Mem::Type* get_type() { return nullptr; }
		// covert args' type in order to comply with params' type.
		virtual Mem::Object run(std::vector<Mem::Object>& objs) { return nullptr; }
		void set_index(size_t pos);

		bool is_completed;
		int index;
	protected:
		std::vector<Mem::Type*> param_types;
	};
	class Function : public FunctionBase
	{
	public:
		Function(Mem::Type* t, Param *p, Block *bl = nullptr) ;
		Function(Mem::Type* t, Block *bl = nullptr);
		void reset_block(Block *b);
		void reser_param(Param *p);
		Param *param;
		Mem::Object run(std::vector<Mem::Object> &objs)override;
		Block *get_block() { return blo; }
		Mem::Type* get_type()override { return type; }
		
	private:
		Mem::Type* type;
		Block *blo;
	};
	class SystemFunction :public FunctionBase 
	{
	public:
		SystemFunction(Mem::Type* t, const std::function<Mem::Object(std::vector<Mem::Object>&)> &fun) :type(t), func(fun) {}
		Mem::Object run(std::vector<Mem::Object> &objs)override
		{
			return func(objs);
		}
		void convert_arg(std::vector<Expr*> &args)override{}
		Mem::Type* get_type()override
		{
			return type;
		}
		bool check_param(const std::vector<size_t>& types)override;
		void dnt_check_param()
		{
			check_param_type = false;
		}
	private:
		Mem::Type* type;
		bool check_param_type = true;
		std::function<Mem::Object(std::vector<Mem::Object>&)>func;
	};

	namespace Parser
	{
		Param *build_param();
		void build_function();
	}
	extern Block *current_function_block;
	extern Mem::Type* this_func_type;
	extern std::map<std::string, Function*> function_table;
}