#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "expr.hpp"
namespace Mer
{
	using ParamFeature = std::vector<size_t>;
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

		virtual void check_param(const std::vector<size_t>& types);
		virtual void convert_arg(std::vector<ParserNode*> &args);
		void set_param_types(const std::vector<size_t>& types)
		{
			param_types = types;
		}
		virtual size_t get_type() { return 0; }
		// covert args' type in order to comply with params' type.
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
		Function(size_t t, Param *p);
		Function(size_t t);
		void reser_param(Param *p);
		Param *param=nullptr;
		Mem::Object run(std::vector<Mem::Object> &objs)override;
		size_t get_type()override { return type; }
		void set_function_block();
		std::vector<ParserNode*> stmts;
		size_t* pc=new size_t(0);
	private:
		size_t param_size;
		size_t type;
	};
	class SystemFunction :public FunctionBase 
	{
	public:
		SystemFunction(size_t t, const std::function<Mem::Object(std::vector<Mem::Object>&)> &fun) :type(t), func(fun) {}
		Mem::Object run(std::vector<Mem::Object> &objs)override
		{
			return func(objs);
		}
		void convert_arg(std::vector<ParserNode*> &args)override{}
		size_t get_type()override
		{
			return type;
		}
		void check_param(const std::vector<size_t>& types)override;
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
		// build param type_code vec;
		ParamFeature build_param_feature();
		// build param which contains nameinfo and type_code;
		Param* build_param();
		void build_function();
		std::pair<std::string,Function*> _build_function();
	}
	extern Block *current_function_block;
	extern bool is_struct_member_function;
	extern std::map<std::string, Function*> function_table;
}