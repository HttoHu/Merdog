#pragma once
#include <vector>
#include <map>
#include "var.hpp"
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
======================================ideas==================================
we create the structure object on heap ,so first of all we should build a heap memory. 

* Maybe a little bit hard for me. 
*/
namespace Mer
{
	class ParserNode;
	class CompoundObject :public Mer::Mem::Value
	{
	public:
		CompoundObject(size_t _type, const std::vector<Mem::Object> &_objs):type(_type),objs(_objs) {
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
	class Structure
	{
	public:
		Structure(const std::map<std::string, std::pair<size_t,size_t>> &_mem_table) :mem_table(_mem_table) 
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
		std::pair<size_t,size_t> find_var(const std::string &str) {
			auto result=mem_table.find(str);
			if (result != mem_table.end())
				return result->second;
			throw Error(str + " no found");
		}
	private:
		// first is position, and second is type;
		std::vector<size_t> member_block;
		std::map<std::string, std::pair<size_t,size_t>> mem_table;
	};
	namespace Parser
	{
		ParserNode *parse_id();
		Structure* parse_structure();
	}
}