#pragma once
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

/*
* Const Value , Value , Vars ,Function Call
*/
#include "./parser_node.hpp"

#include <cstring>
#include "./lexer.hpp"

namespace Mer {
	size_t get_type_size(type_code_index ty);

	class LConV :public ParserNode {
	public:
		LConV(char* _val, type_code_index ty) :ParserNode(NodeType::LConV), len(get_type_size(ty)), type(ty) { val = new char[len]; memcpy(val, _val, len); }
		LConV(Token* tok);
		template<typename T>
		LConV(const T& t) {
			if (t == typeid(int_default))
				type = (int)BasicTypeTag::INT;
			else if (t == typeid(real_default))
				type = (int)BasicTypeTag::REAL;
			else if (t == typeid(byte_default))
				type = (int)BasicTypeTag::BYTE;
			else
				throw Error("invalid literal-const value type");
			val = new char[sizeof(t)];
			*(T*)(val) = t;
			len = sizeof(t);
		}
		type_code_index get_type()const override { return type; }
		size_t need_space()override { return len; }
		bool constant()const override { return true; }
		void execute(char* ret)override { memcpy(ret, val, len); }
		ParserNode* clone()override { return new LConV(val, type); }
		std::string to_string()const override;
		~LConV() { delete[] val; }
	private:
		type_code_index type = 0;
		int len;
		char* val;
	};
	class Variable:public ParserNode {
	public:
		Variable(const std::string &_var_name,type_code_index _type, size_t _pos, size_t* _base_ptr);
		void execute(char* ret) override;
		size_t get_pos() override { return pos; }
		char* get_runtime_pos()override;
		type_code_index get_type()const override { return type; }
		std::string to_string()const override;
	private:
		size_t len;
		std::string var_name;
		type_code_index type;
		size_t pos;
		size_t* base_ptr;
	};
	class Print : public ParserNode{
	public:
		Print(ParserNode* _node);
		void execute(char* ret)override;
		std::string to_string()const override;
	private:
		ParserNode* node;
	};
	class ArrayIndex : public ParserNode {
	public:
		// take place the ownership of origins elements
		ArrayIndex(size_t _pos,type_code_index _ty,size_t*_base_ptr, const std::vector<size_t> & suffix,
			const std::vector<ParserNode*> &origins);
		type_code_index get_type()const override { return type; }
		std::string to_string()const override;
		void execute(char*)override;
		size_t get_pos()override;
		size_t need_space()override;
		size_t actual_space()override;

	private:
		size_t pos;
		size_t* base_ptr;
		ParserNode* index;
		type_code_index type;
		size_t type_len;
	};
}