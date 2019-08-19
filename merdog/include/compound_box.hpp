#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include "var.hpp"
#include "parser.hpp"
namespace Mer
{
	class Id;
	using TVar=std::pair<size_t, Mem::Object>;
	using TName=std::pair<std::string, size_t>;
	/*class CompoundBox
	{
	public:
		using SIM=std::map<std::string, int>;
		using DataTable=std::vector<TVar>;
		CompoundBox(const DataTable & _c, SIM & _m):
			children_table(_c), mapping(_m) {}
		TVar find(int index) {
			return children_table[index];
		}
	private:
		DataTable children_table;
		SIM &mapping;
	};*/
	class UStructrue
	{
	public:
		using SIM=std::map<std::string, int>;
		void push_new_children(size_t t, std::string id_name)
		{
			mapping.insert({ id_name,be++ });
			STMapping.insert({ id_name,t });
		}
		std::vector<Mem::Object> init();
		SIM mapping;
		std::map<std::string, size_t> STMapping;
		int be = 0;
	};
	class USObject :public Mem::Value {
	public:
		USObject(const std::vector<Mem::Object>& _vec):vec(_vec) { }
		virtual Mem::Object operator[](Mem::Object v)
		{
			return vec[std::static_pointer_cast<Mem::Int>(v)->get_value()];
		}
	private:
		std::vector<Mem::Object> vec;
	};
	class StructureDecl :public ParserNode
	{
	public:
		StructureDecl(size_t t, const std::vector<Token*>& v);
		Mem::Object execute()override;
	private:
		size_t type;
		std::vector<std::size_t> var_list;
	};
	extern std::map<std::string, UStructrue*> ustructure_map;
	extern std::map<std::string, size_t> name_type_mapping;
	extern std::map<size_t, std::string> type_name_mapping;
	void build_ustructure();
	StructureDecl* structure_decl();
	Mer::UStructrue* find_ustructure_t(size_t type);

}