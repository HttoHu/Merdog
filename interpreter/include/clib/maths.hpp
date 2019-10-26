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
#include "merdog_io.hpp"
// offer some common maths method.
namespace Mer
{
	class Namespace;
	extern Namespace *maths;
	void set_maths();
	extern Mer::SystemFunction *min;
	extern Mer::SystemFunction *mod;
	extern Mer::SystemFunction *sqrt;
	extern Mer::SystemFunction *sin;
	extern Mer::SystemFunction *cos;
	extern Mer::SystemFunction *tan;
	extern Mer::SystemFunction *arsin;
	extern Mer::SystemFunction *arcos;
	extern Mer::SystemFunction *artan;
	extern Mer::SystemFunction *abs;
}