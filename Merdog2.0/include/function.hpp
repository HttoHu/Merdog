#pragma once
#include <functional>
#include "parser.hpp"
namespace Mer
{
	class ParVar :public AST
	{
	public:
		ParVar(size_t p,Expr*e) :pos(p),expr(e) {}
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

	};
	class FunctionBase
	{
	public:
		virtual Mem::Object call(std::vector<AST*> &arg)=0;
		virtual std::vector<size_t> get_param()=0;
		virtual ~FunctionBase() = default;
	};
	class Function:public FunctionBase
	{
	public:
		Mem::Object call(std::vector<AST*> &arg)override;
		std::vector<size_t> get_param()override
		{
			std::vector<size_t> ret;
			for (auto &a : param->param)
			{
				ret.push_back(a->pos);
			}
			return ret;
		}
		Param * param;
		Block *blo;
	};
	extern std::map<std::string, size_t> function_map;
	extern std::vector<FunctionBase*> function_list;
	class SystemFunction :public FunctionBase
	{
	public:
		SystemFunction(std::function<Mem::Object(std::vector<AST*>&)>& fun) :func(fun) {}
		Mem::Object call(std::vector<AST*>&arg)override
		{
			return func(arg);
		}
	private:
		std::function<Mem::Object(std::vector<AST*>&)>func;
	};
	namespace Parser
	{
		Param *build_param();
		ParamPart *build_param_part();
		void build_function();
	}
}