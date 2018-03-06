/*
*		Inspired by
*		https://ruslanspivak.com/lsbasi-part10/
*		Ruslan's Blog
*		C++ Version.
*		Yuantao Hu 2018
*		Email :Huyuantao@outlook.com
*/
#pragma once
#include <functional>
#include "parser.hpp"
namespace Mer
{
	class ParVar :public AST
	{
	public:
		ParVar(size_t p, Expr*e);
		Mem::Object get_value();
	private:
		size_t pos;
		Expr *expr;
	};
	class ParamPart
	{
	public:
		ParamPart(Type *type, Token *tok);
		ParVar* create_var(Expr *exp);
		size_t pos;
		Type *t;
	};
	class Param
	{
	public:
		std::deque <AST*> generate_statement(std::vector<Expr*> &v);
		std::vector <ParamPart*> param;
		std::vector<size_t> get_param()
		{
			std::vector<size_t> ret;
			for (auto &a :param)
			{
				ret.push_back(a->pos);
			}
			return ret;
		}
	};
	class FunctionBase
	{
	public:
		virtual Mem::Object call(std::vector<Mem::Object> &arg,int reserve_size)=0;
		virtual ~FunctionBase() = default;
		virtual size_t get_type()
		{
			return 0;
		}
	};
	class Function:public FunctionBase
	{
	public:
		Function(size_t t):type_code(t) {}
		Mem::Object call(std::vector<Mem::Object> &arg,int reserve_size)override;
		size_t get_type()override
		{
			return type_code;
		}
		std::vector<size_t> param;
		Block *blo;
	private:
		size_t type_code;
	};             

	extern std::map<std::string, size_t> function_map;
	extern std::vector<FunctionBase*> function_list;
	class SystemFunction :public FunctionBase
	{
	public:
		SystemFunction(size_t t, const std::function<Mem::Object(std::vector<Mem::Object>&)> &fun) :type(t),func(fun) {}
		Mem::Object call(std::vector<Mem::Object>&arg,int reserve_size)override
		{
			return func(arg);
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
		ParamPart *build_param_part();
		void build_function();
	}
}