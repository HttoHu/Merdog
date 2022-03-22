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
#include "lexer.hpp"
#include <utility>
#include <memory>
namespace Mer
{
	class ParserNode;
	using UptrPNode = std::unique_ptr<ParserNode>;
	using PosPtr = std::shared_ptr<size_t>;
	extern std::string output_buff;
	std::string run_interpreter(const std::string&file_content);
	extern std::map<std::string, void(*)()>repository;
	extern std::vector<size_t *> _pcs;
	extern std::vector<std::pair<PosPtr, PosPtr>> _nearest_loop_pos;
	/*
		when you call a struct-member function as follows (sps<-structure_parent_stack)
		co.
		* sps.push_back(co);
		call	distance(sps.back());
		* sps.pop_back();
	*/
	extern std::vector<ParserNode*> structure_parent_stack;
	void new_loop(PosPtr a,PosPtr b);
	void end_loop();
	PosPtr& loop_start();
	PosPtr& loop_end();
}