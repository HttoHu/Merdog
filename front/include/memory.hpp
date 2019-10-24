/*
	MIT License

	Copyright (c) 2019 Htto Hu

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
#include <iostream>
#define MERDOG_MEM_SIZE 4096*10
#define LIT_END_POS 4096;
namespace mer
{
	class Memory
	{
	public:
		Memory();
		char* operator[](int index)
		{
			return mem + index;
		}
		~Memory()
		{
			delete[]mem;
		}
		
	private:
		int size = MERDOG_MEM_SIZE;
		void alloc();
		char* mem;
	};
	extern Memory memory;
	template<typename _Ty>
	_Ty& data_cast(int sz) {
		return *(_Ty*)(memory[sz]);
	}
}