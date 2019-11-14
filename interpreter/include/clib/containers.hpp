/*
		MIT License

		Copyright (c) 2019 ºúÔ¶èº

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
			Vector() {}
			Vector(int count, Mem::Object obj);
			Vector(const std::vector<Mem::Object>& objs) :content(objs) {}
			Vector(std::vector<Mem::Object>&& objs) :content(objs) {}
			Mem::Object operator[](Mem::Object index)override;
			Mem::Object clone()const override;
			std::vector<Mem::Object> content;
		};
		void using_vector();
		void register_new_vector_type(size_t element_type);
	}
}