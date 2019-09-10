#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include "object.hpp"
#include "parser.hpp"
namespace Mer
{
	class Id;
	using TVar=std::pair<size_t, Mem::Object>;
	using TName=std::pair<std::string, size_t>;

	class UStructure
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
		Mem::Object operator=(Mem::Object v)override
		{
			// haha, lasy and high performance.
			auto a = std::static_pointer_cast<USObject>(v);
			vec = a->vec;
			return a;
		}
		virtual Mem::Object operator[](Mem::Object v)
		{
			return vec[std::static_pointer_cast<Mem::Int>(v)->get_value()];
		}
	private:
		std::vector<Mem::Object> vec;
	};


	class DefaultInitList :public ParserNode
	{
	public:
		DefaultInitList(const std::map<std::string, int>& m);
		Mem::Object execute()override;
	private:
		std::vector<Mem::Object> vec;
	};
	class StructureInitList:public ParserNode
	{
	public:
		StructureInitList(const std::map<std::string, int>& m);
		// the purpose of setting t is that it can distinguish bewtween init_without argument and init_with argument.
		Mem::Object execute()override;
		virtual ~StructureInitList() {}
	private:
		std::vector<Expr*> vec;
	};


	extern std::map<std::string, UStructure*> ustructure_map;
	extern std::map<size_t, std::string> type_name_mapping;
	void build_ustructure();
	
	//StructureDecl* structobj_decl();
	Mer::UStructure* find_ustructure_t(size_t type);
	
}