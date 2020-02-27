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
		using MapContent = std::map<Mem::Object, Mem::Object, _compare_operator>;
		class Set :public Mem::Value
		{
		public:
			Set(SetContent&& sc) :data(sc) {}
			Set(const SetContent& sc) :data(sc) {}
			Set(type_code_index element_type,const std::vector<Mem::Object>& vec);
			Set(type_code_index element_type);
			static type_code_index type_code;
			Mem::Object operator[](Mem::Object)override;
			Mem::Object clone()const override;
			SetContent data;
		};
		class Map :public Mem::Value
		{
		public:
			Map(const MapContent& sc) :data(sc) {}
			Map(type_code_index key);
			static type_code_index type_code;
			Mem::Object operator[](Mem::Object)override;
			Mem::Object clone()const override;
			MapContent data;
		};
	}
	void using_set();
	void using_map();
	void register_set(type_code_index element_type);
}