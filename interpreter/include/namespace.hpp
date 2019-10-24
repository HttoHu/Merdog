#pragma once
#include "expr.hpp"
#include "function.hpp"
#define tsymbol_table this_namespace->sl_table
namespace Mer
{
	class Structure;
	class SymbolTable;
	class Namespace
	{
	public:
		Namespace(Namespace* pare);
		Namespace(Namespace* pare, 
			const std::map<std::string,Namespace*>&cs,
			const std::map<std::string, FunctionBase*> &funcs):parent(pare), children(cs), functions(funcs) {}
		Namespace *parent;
		std::map<std::string, std::pair<Structure*, size_t>> structures;
		std::map<std::string, Namespace*> children;
		std::map<std::string, FunctionBase*> functions;
		std::vector<Namespace*> using_namespaces;
		SymbolTable *sl_table;
		void set_new_func(const std::string &name, size_t 
			, FunctionBase* func);
		void set_new_var(const std::string &name,size_t type, Mem::Object obj);


		Mem::Object find_var(const std::string &name);
		FunctionBase *find_func(const std::string &name);
	};
	// 5-3 
	/*
	Task : 
	1. Namespace 1.0
	*/
	extern Namespace *root_namespace;
	extern Namespace *this_namespace;
	/* 
	* a class which is aimed to change the value of namespace_var,
	* the global variable also called namespace var in merdog, is differnt from local var. At the stage of 
	* syntax analysis, the local var will convert to a postion, however, global var will created in parser 
	* stage, so global var always be a object, and the Assign classes don't handle such situation. 
	*/
	namespace Parser
	{
		Mer::Namespace * _find_namespace_driver(Mer::Namespace *current, const std::string &name);
		Namespace *find_namespace(const std::string &name);
		Namespace *build_namespace();
		void create_namespace_var();
	}

}