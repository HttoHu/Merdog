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
#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include "basic_objects.hpp"

#include "parser.hpp"
/* 
	from 1-7 is basic type 
	when the type code is even, it means that the type is a pointer
	for instance 1 -> int, 2 -> int *.
*/
#define USER_TYPE_INDEX 10
namespace Mer
{
	class Id;
	class FunctionBase;
	using TVar=std::pair<size_t, Mem::Object>;
	using TName=std::pair<std::string, size_t>;
	// the class to record the struct infomation. 
	class UStructure
	{
	public:
		using SIM=std::map<std::string, int>;
		// add new member
		void push_new_children(size_t t, std::string id_name);
		// first: member type second: member pos
		std::pair<type_code_index, size_t > get_member_info(std::string member_name);
		std::vector<Mem::Object> init();
		// get the member position by the name of it.
		SIM mapping;
		// get the member type by the name of it. Take care, you should be aware that mapping and STMapping is totally different.
		std::map<std::string, type_code_index> STMapping;
		void push_init(Mem::Object obj) { init_vec.push_back(obj); }
		void print();
		WordRecorder* find_id_info(const std::string &id);
		std::vector<Mem::Object> init_vec;
		std::map<std::string, WordRecorder*> structure_member_table;
	private:

		friend void build_ustructure();
		// the index to new member is used in build phase. 
		int be = 0;
	};
	// an object which create user-def struct obj.
	class USObject :public Mem::Value {
	public:
		USObject(const std::vector<Mem::Object>& _vec):vec(_vec) { }
		// assign , init etc.
		Mem::Object operator=(Mem::Object v)override;
		Mem::Object clone()const override;
		/*
			user-def struct obj is a single obj, and its members are all pushed into a vector<Object>. When you need obtain the member 
			of user-def struct obj, you need get its postion of the struct.
		*/
		std::string to_string()const override;
		Mem::Object operator[](Mem::Object v)override
		{
			return vec[std::static_pointer_cast<Mem::Int>(v)->get_value()];
		}
	private:
		std::vector<Mem::Object> vec;
	};


	/*
		for instance 
		= merdog code =
		struct Coor
		{
			....
		}
		Coor c;
		it needs DefaultInitList to init the c. 
	*/


	class DefaultInitList :public ParserNode
	{
	public:
		DefaultInitList(type_code_index type);
		Mem::Object execute()override;
		ParserNode* clone()override;
	private:
		DefaultInitList() {}
		std::vector<Mem::Object> vec;
	};
	// the purpose of two class t is to distinguish bewtween init_without argument and init_with argument.
	class StructureInitList:public ParserNode
	{
	public:
		StructureInitList(UStructure *us, type_code_index _type_code=0);
		type_code_index get_type()override{
			return type_code;
		}
		Mem::Object execute()override;
		virtual ~StructureInitList() {}
		ParserNode* clone()override;
	private:
		StructureInitList() {}
		std::vector<ParserNode*> vec;
		type_code_index type_code=0;
	};
	class MemberVar :public ParserNode
	{
	public:
		MemberVar(size_t pos, type_code_index _type) :member_pos(pos), type(_type) {}
		type_code_index get_type()override { return type; }
		Mem::Object execute()override;
		ParserNode* clone()override {
			return new MemberVar(member_pos, type);
		}
	private:
		size_t member_pos;
		type_code_index type;
	};
	// you can get the struct_info by its name
	extern std::map<std::string, UStructure*> ustructure_map;
	// get the struct name by its type code.

	extern std::map<type_code_index, std::map<std::string, FunctionBase*>> member_function_table;
	// to build the struct , records different information and push the struct to the ustructure_map
	void build_ustructure();
	extern std::map<type_code_index, std::string> type_name_mapping;
	/*
		MemberVar need a Object to execute operator[] , which obtain from parents_vec.push_back()
		the parents_vec managed by member function call, when they call, they push a new Object which represent the parent 
		ready to call its member function.
	*/
	extern std::vector<Mem::Object> parents_vec;
	Mer::UStructure* find_ustructure_t(type_code_index type);
	// the map which store the compare operator to 
	extern std::map<type_code_index, FunctionBase*> comparison_map;
}