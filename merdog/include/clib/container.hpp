#pragma once
#include "../structure.hpp"
#include "sys_method.hpp"
#include <vector>
namespace Mer
{
	class VectorObject :public Mem::Value
	{
	public:
		VectorObject(size_t 
			,int size);
		VectorObject(size_t type,const std::vector<Mem::Object>&  vec);
		Mem::Object operator[](Mem::Object v)override;
	private:
		size_t obj_type;
		std::vector<Mem::Object> content;
	};
	class mVector:public StructureBase
	{
	public:
		FunctionBase * get_function(const std::string &id) override;
		void push_functions(const std::string &str, SystemFunction* func)
		{
			metholds_map.insert({ str, func });
		}
	private:
		std::map<std::string, SystemFunction*> metholds_map;
	}merdog_vector;
	// register vector, you can't use vector before enable it.
	void using_vector();
}