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
#pragma once
#include "expr.hpp"
#include "function.hpp"
#define tsymbol_table root_namespace->sl_table
namespace Mer
{
	class Structure;
	class SymbolTable;
	class Namespace
	{
	public:
		Namespace(Namespace* pare);
		Namespace(Namespace* pare, 
			const std::map<std::string,Namespace*>&cs):parent(pare), children(cs) {}
		Namespace *parent;
		std::map<std::string, std::pair<Structure*, type_code_index>> structures;
		std::map<std::string, Namespace*> children;
		std::vector<Namespace*> using_namespaces;
		SymbolTable* sl_table = nullptr;
		void set_new_func(const std::string & name, FunctionBase * func);
		void set_new_var(const std::string &name,size_t type, Mem::Object obj);
		Mem::Object find_var(const std::string &name);
		~Namespace();
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
	}

}