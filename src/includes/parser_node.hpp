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
#include <memory>
#include <iostream>
#include "./error.hpp"
#include "./type.hpp"

namespace Mer
{
	enum class NodeType :int {
		BIN_OP, LConV,LOGICAL_BIN,UNARY_OP,
		PRINT,CAST,
		VAR_DECL,
		EXPR,ASSIGN,VAR,
		GOTO,CONTINUE,IFFJ,IFTJ
	};
	// the node of an AST, every node is excutable.
	// 
	// if you want to run the code ,just do a dfs execution.
	class ParserNode
	{
	public:
		ParserNode(NodeType nt) :node_type(nt) {}
		NodeType get_node_type() { return node_type; }
		size_t node_size() {
			return type_tab[get_type()]->type_length();
		}
        virtual ~ParserNode() {}
	public:
        virtual char* get_runtime_pos(){return nullptr;}
		virtual size_t get_pos() { return -1; }
		// the space to calculate the node, the data may write to memory temporary..
		virtual size_t need_space() { return 0; }
		// if you decl a var, the node will occupy the space not temporary.
		virtual size_t actual_space() { return 0; }
		
		virtual std::string to_string()const 
		{
			return "<empty_node>";
		}
		// to known if the node can get result before code running.
		virtual bool constant()const
		{
			return false;
		}
		// every node has a type in order to do static type check.
		virtual type_code_index get_type()const
		{
			return 0;
		}
		// write the result to the specified place.
		virtual void execute(char* ret)
		{
			throw std::runtime_error("runtime error: called by an indefinite var");
		}
		// clone is not overrided by all children. 
		virtual ParserNode* clone() {
			throw Error("cloned in ParserNode");
		}
	public:
		NodeType node_type;
	};
	using UptrPNode = std::unique_ptr<ParserNode>;
}
