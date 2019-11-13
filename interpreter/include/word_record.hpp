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
#include "value.hpp"
#include <functional>
namespace Mer
{
	enum ESymbol
	{
		SFUN,SVAR,SNAME,
		SGVAR,SGARR,
		SSTRUCTURE,SARRAY,
		STYPE,USVAR,MVAR,
		SCONTAINER,
	};
	struct WordRecorder
	{
	public:
		WordRecorder(ESymbol e, size_t tc = 0) :es(e),type_code(tc) {}
		ESymbol es;
		int count = 1;
		virtual size_t get_pos() { return 0; }
		virtual std::string to_string() { return""; }
		size_t get_type() { return type_code; }
		virtual ~WordRecorder() {}
	protected:
		size_t type_code;
	};
	struct VarIdRecorder :public WordRecorder
	{
	public:
		VarIdRecorder(size_t type,size_t p,WordRecorder wr=SVAR) 
			:WordRecorder(wr),pos(p)
		{
			type_code = type;
		}
		size_t pos;
		size_t get_pos()override {
			return pos + mem.get_current();
		}
		std::string to_string()override
		{
			return "pos:" + std::to_string(pos);
		}
		// to get array size;
	private:
	};
	struct MVarRecorder :public WordRecorder
	{
		MVarRecorder(size_t type,size_t _pos) :pos(_pos), WordRecorder(MVAR,type) {}
		size_t get_pos()override { return pos; }
		std::string to_string()override { return "MVar :" + std::to_string(pos); }
		size_t pos;
	};
	struct StructRecorder :public WordRecorder
	{
	public:
		StructRecorder() :WordRecorder(ESymbol::SSTRUCTURE)
		{
			 Mem::type_counter+=2;
			 type_code = (size_t)Mem::type_counter;
		}

	};
	class Namespace;
	struct GVarIdRecorder:public WordRecorder
	{
	public:
		GVarIdRecorder(size_t t,size_t _pos,ESymbol e=SGVAR) :WordRecorder(e,t), pos(_pos) {
		}
		bool& is_arr() { return arr; }
		size_t get_pos()override
		{
			return pos;
		}
		Mem::Object& get_value()
		{
			return mem[pos];
		}
		size_t pos;
		bool arr=false;
	};
	struct FuncIdRecorder :public WordRecorder
	{
	public:
		FuncIdRecorder(size_t type) :WordRecorder(ESymbol::SFUN){
			type_code = type;
		}
	private:
	};
	struct ContainerTypeRecorder :public WordRecorder
	{
	public:
		ContainerTypeRecorder(const std::string &str):WordRecorder(SCONTAINER)
		{
			type_code = Mem::type_counter += 2;
		}
		std::string to_string() override { 
			return "type:"+type_name; 
		}
	private:
		std::string type_name;
	};
	class SymbolTable
	{
	public:
		SymbolTable()
		{
			data.push_front(std::map<std::string, WordRecorder*>());
		}
		void new_block()
		{
			data.push_front(std::map<std::string, WordRecorder*>());
		}
		void end_block()
		{
			data.pop_front();
		}
		void type_check(Token *id, ESymbol e)
		{
			auto result = find(Id::get_value(id));
			if (result == nullptr)
				throw Error("id " + id->to_string() + " no found.");
			if (result->es != e)
				throw Error("id type not matched");
		}
		WordRecorder* find_front(std::string id)
		{
			auto result = data.front().find(id);
			if (result == data.front().end())
				return nullptr;
			return result->second;
		}
		WordRecorder* find(std::string id)
		{
			for (size_t i=0;i<data.size();i++)
			{
				auto result = data[i].find(id);
				if (result != data[i].end())
				{
					return result->second;
				}
			}
			return nullptr;
		}
		void push_glo(std::string id, WordRecorder *wr)
		{
			data.back().insert({ id,wr });
		}
		void push(std::string id, WordRecorder *wr)
		{
			data.front().insert({ id,wr });
		}
		void print();
	private:
		std::deque<std::map<std::string, WordRecorder*>> data;
	};
}
