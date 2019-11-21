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
/*

*/
#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "expr.hpp"
namespace Mer
{
	// arguments types list
	using _compare_operator =std::function<bool(Mem::Object,Mem::Object)>;
	using ParamFeature = std::vector<size_t>;
	using _intern_func = std::function<Mem::Object (const std::vector<Mem::Object>&)>;
	bool compare_param_feature(const std::vector<size_t>& p1, const std::vector<size_t>& p2);
	std::string param_feature_to_string(const ParamFeature& pf);
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
		ParamFeature get_param_feature();
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
		virtual Mem::Object run(const std::vector<Mem::Object>& objs) { return nullptr; }
		std::vector<size_t> param_types;
		bool is_completed=false;
		bool is_check_type() { return check_param_type; }
		std::string to_string(std::string name="no_name_func")const ;
	protected:
		bool check_param_type = true;

	};
	class Function : public FunctionBase
	{
	public:
		Function(size_t t, Param *p);
		Function(size_t t);
		void reser_param(Param *p);
		Param *param=nullptr;
		Mem::Object run(const std::vector<Mem::Object> &objs)override;
		size_t get_type()override { return type; }
		void set_function_block();
		std::vector<ParserNode*> stmts;
		size_t* pc=new size_t(0);
		size_t off;
	private:

		size_t param_size;
		size_t type;
	};
	class SystemFunction :public FunctionBase 
	{
	public:
		SystemFunction(size_t t, _intern_func fun) :type(t), func(fun) {}
		Mem::Object run(const std::vector<Mem::Object> &objs)override
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

		_intern_func func;
	};
	class InitKey
	{
	public:
		InitKey(size_t type_c, const std::vector<size_t>& param) :type_code(type_c), params(param) {}
		size_t type_code;
		std::vector<size_t> params;
		bool operator<(const InitKey& init_key)const;
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
	extern std::map<std::string, Function*> function_table;
	extern std::map<InitKey, FunctionBase*> type_init_function_map;
	extern std::map<size_t, Mem::Object> type_init_map;
	extern std::map<size_t, _compare_operator> compare_map;
}