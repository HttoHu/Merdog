#pragma once
#include <functional>
#include <iostream>
#include "../basic_objects.hpp"
namespace Mer
{


	using _compare_operator = std::function<bool(Mem::Object, Mem::Object)>; 
	extern std::map<type_code_index, _compare_operator> compare_map;

	_compare_operator find_compare_operator(type_code_index s);
	namespace Container
	{
		using SetContent = std::set<Mem::Object, _compare_operator>;
		class Set :public Mem::Value
		{
		public:
			Set(const SetContent& sc) :data(sc) {}
			Set(type_code_index element_type);
			static type_code_index type_code;
			Mem::Object operator[](Mem::Object)override;
			Mem::Object clone()const override;
			SetContent data;
		};
	}
	void using_set();
	void register_set(type_code_index element_type);
}