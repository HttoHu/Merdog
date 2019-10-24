#pragma once
#include "../function.hpp"
namespace Mer
{

	class SysMethold:public FunctionBase
	{
	public:
		SysMethold(size_t t, const std::function<Mem::Object(std::vector<Mem::Object>&)> &fun) :type(t), func(fun) {}
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
		Mem::Object struct_obj;
	};
}