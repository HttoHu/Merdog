#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include "object.hpp"
#include "parser.hpp"
/* 
	from 1-7 is basic type 
	when the type code is even, it means that the type is a pointer
	for instance 1 -> int, 2 -> int *.
*/
#define USER_TYPE_INDEX 8
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
		// first: type second: off_set
		std::pair<size_t, size_t > get_member_info(std::string member_name);
		std::vector<Mem::Object> init();
		// get the member position by the name of it.
		SIM mapping;
		// get the member type by the name of it. Take care, you should be aware that mapping and STMapping is totally different.
		std::map<std::string, size_t> STMapping;
		void push_init(Mem::Object obj) { init_vec.push_back(obj); }
		WordRecorder* find_id_info(const std::string &id);
		std::map<std::string, FunctionBase*> member_function_table;
	private:
		std::map<std::string, WordRecorder*> structure_member_table;
		std::vector<Mem::Object> init_vec;
		// the index to new member is used in build phase. 
		friend void build_ustructure();
		int be = 0;
	};
	// an object which create user-def struct obj.
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
		DefaultInitList(size_t type);
		std::vector<ParserNode*>& get_exprs() { return vec; }
	private:
		std::vector<ParserNode*> vec;
	};
	// the purpose of two class t is to distinguish bewtween init_without argument and init_with argument.
	class StructureInitList:public ParserNode
	{
	public:
		StructureInitList(const std::map<std::string, int>& m,size_t _type_code=0);
		size_t get_type()override{
			return type_code;
		}
		std::vector<ParserNode*>& get_exprs() { return vec; }
		virtual ~StructureInitList() {}
	private:
		std::vector<ParserNode*> vec;
		size_t type_code;
	};
	// you can get the struct_info by its name
	extern std::map<std::string, UStructure*> ustructure_map;
	// get the struct name by its type code.
	extern std::map<size_t, std::string> type_name_mapping;
	// to build the struct , records different information and push the struct to the ustructure_map
	void build_ustructure();
	
	Mer::UStructure* find_ustructure_t(size_t type);
	// if struct haven't found, return nullptr
	Mer::UStructure* find_ustructure(size_t type);
	
}