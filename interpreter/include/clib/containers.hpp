#pragma once
#include<iostream>
#include<vector>
#include "../basic_objects.hpp"
namespace Mer
{
	namespace Container
	{
		class Vector :public Mem::Value
		{
		public:
			static int vector_type_code;
			Vector(int count, Mem::Object obj);
			Vector(const std::vector<Mem::Object>& objs) :content(objs) {}
			Vector(std::vector<Mem::Object>&& objs) :content(objs) {}
			Mem::Object operator[](Mem::Object index)override;
		private:
			std::vector<Mem::Object> content;
		};
		void using_vecotr();
		void register_new_vector_type(size_t element_type);
	}
}