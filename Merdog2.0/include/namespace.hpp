#pragma once
#include "function.hpp"
namespace Mer
{
	class Namespace
	{
	public:
	private:
		Namespace *parent;
		std::map<std::string, Namespace*> childrens;
		std::map<std::string, Function*> user_functions;
		std::map<std::string, Mem::Object*> namespace_vars;
	};
}