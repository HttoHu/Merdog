/*
		MIT License

		Copyright (c) 2022 HttoHu

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
#include <string>
#include <iostream>
#include "./error.hpp"
#include "./type.hpp"

namespace Mer
{
	// the node of an AST, every node is excutable.
	// if you want to run the code ,just do a dfs execution.
	class ParserNode
	{
	public:
		ParserNode(){}
		virtual size_t get_pos() { return 0; }
		virtual ~ParserNode() {}

		virtual std::string to_string()
		{
			return "<empty_node>";
		}
		virtual bool constant()const
		{
			return false;
		}
		virtual int get_type()
		{
			return 0;
		}
		virtual char* execute() 
		{
			throw std::runtime_error("runtime error: called by an indefinite var");
		}
		// clone is not overrided by all children. 
		virtual ParserNode* clone() {
			throw Error("cloned in ParserNode");
		}
	};
	using UptrPNode = std::unique_ptr<ParserNode>;
}
