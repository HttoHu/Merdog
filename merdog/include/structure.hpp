#pragma once
#include <vector>
#include <map>
#include "parser.hpp"
/*
* structure
* 2018 5-14
* By Htto eccentric mind... 
* preview:
	namespace Htto
	{
		struct Coor
		{
			int x;
			int y;
			method int distance(Coor c2)
			{
				return std.maths.sqrt((x-c2.x)*(x-c2.x)+(y-c2.y)*(y-c2.y));
			}
		}
	}
	Coor x=[Coor]{};
======================================ideas==================================
we create the structure object on heap ,so first of all we should build a heap memory. 
* Maybe a little bit hard for me. 
*/
namespace Mer
{
	class Type
	{
	public:
		Type(size_t _size, std::string _name, size_t _id_pos) :size(_size), name(_name), id_pos(_id_pos) {}
		virtual bool is_structure() { return false; }
		size_t type_size() { return size; }
		std::string type_name() { return name; }
		size_t type_id() { return id_pos; }
		virtual ~Type() {}
	private:
		size_t size;
		std::string name;
		size_t id_pos;
	};
	extern std::vector<Type*> type_vtable;
	class CompoundObject :public Mer::Mem::Value
	{
	public:
		CompoundObject(size_t _type, const std::vector<Mem::Object> &_objs):type(_type),objs(_objs) 
		{
		}
		Mem::Object member(size_t index) { return objs[index]; }
		size_t get_type()const override
		{
			return type;
		}
	private:
		size_t type;
		std::vector<Mem::Object> objs;
	};
	class Structure:public Type
	{
	public:
		Structure(std::string name, std::map<std::string, std::pair<size_t,size_t>> &_mem_table):
			Type(_mem_table.size(),name,Mem::type_no()),mem_table(_mem_table) 
		{
			member_block.resize(mem_table.size());
			for (const auto &a : mem_table)
			{
				if (a.second.first < member_block.size())
					member_block[a.second.first] = a.second.second;
				else
					throw Error("<structure.hpp> Line" + std::to_string(__LINE__));
			}
		}
		// first is position, and second is type;
		std::pair<size_t,size_t> find_var(const std::string &str) {
			auto result=mem_table.find(str);
			if (result != mem_table.end())
				return result->second;
			throw Error(str + " no found");
		}
	private:
	
		std::vector<size_t> member_block;
		std::map<std::string, std::pair<size_t,size_t>> mem_table;
	};
	namespace Parser
	{
		Type* find_struct(size_t _type);
		ParserNode *parse_id();
		Structure* parse_structure();
	}
}