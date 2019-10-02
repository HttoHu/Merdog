/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/compound_box.hpp"
#include "../include/lexer.hpp"
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
using namespace Mer;
std::map<std::string, Mer::UStructure*> Mer::ustructure_map;
std::map<size_t,std::string> Mer::type_name_mapping;
//OK
void Mer::build_ustructure()
{
	token_stream.match(STRUCT);
	std::string name = Id::get_value(token_stream.this_token());
	token_stream.match(ID);
	token_stream.match(BEGIN);
	UStructure* us=new UStructure();
	while (token_stream.this_tag() != END)
	{
		size_t type = Mem::get_type_code(token_stream.this_token());
		token_stream.next();
		std::string mem_name = Id::get_value(token_stream.this_token());
		us->push_new_children(type,mem_name);
		token_stream.next();
		token_stream.match(SEMI);
	}
	token_stream.match(END);
	ustructure_map.insert({ name,us });
	Mem::type_counter += 2;
	Mem::type_index.insert({ name,Mem::type_counter });
	type_name_mapping.insert({ Mem::type_counter,name });
	Mer::this_namespace->sl_table->push(name, new WordRecorder(ESymbol::SSTRUCTURE,Mem::type_counter));
	Mem::type_map.insert({ Mem::type_counter ,new Mem::Type(name,Mem::type_counter,{size_t(Mem::type_counter)}) });
}


Mer::UStructure* Mer::find_ustructure_t(size_t type)
{
	auto result = type_name_mapping.find(type);
	if (result == type_name_mapping.end())
		throw Error("struct type " + std::to_string(type) + " undefined");
	auto result2 = ustructure_map.find(result->second);
	if (result2 == ustructure_map.end())
		throw Error("Id " +result2->first + " undefined");
	return result2->second;
}
void Mer::UStructure::push_new_children(size_t t, std::string id_name)
{
	mapping.insert({ id_name,be++ });
	STMapping.insert({ id_name,t });
}
std::vector<Mem::Object> Mer::UStructure::init()
{
	std::vector<Mem::Object> ret;
	for (const auto &a : STMapping)
	{
		ret.push_back(Mem::create_var_t(a.second));
	}
	return ret;
}



Mem::Object Mer::StructureInitList::execute()
{
	std::vector<Mem::Object> obj_vec(vec.size());
	for (size_t i=0;i<vec.size();i++)
	{
		obj_vec[i] = vec[i]->execute();
	}
	return std::make_shared<USObject>(obj_vec);
}

Mer::StructureInitList::StructureInitList(const std::map<std::string, int>& m, size_t _type_code) :vec(m.size()), type_code(_type_code)
{
	token_stream.match(BEGIN);
	size_t last_index=-1;
	while (token_stream.this_tag() != END) {
		auto member_suffix = token_stream.this_token();
		auto result = m.find(Id::get_value(member_suffix));
		if (result == m.end())
			throw Error("struct_obj init Error: Not exits member " + member_suffix->to_string());
		token_stream.match(ID);
		token_stream.match(COLON);
		vec[result->second] = new Expr();
		if (token_stream.this_tag() == END)
		{ 
			token_stream.match(END);
			return;
		}
		token_stream.match(COMMA);
	}
	
}

Mer::DefaultInitList::DefaultInitList(const std::map<std::string, size_t>& m)
{
	for (auto& a : m)
	{
		vec.push_back(Mem::create_var_t(a.second));
	}
}

Mem::Object Mer::DefaultInitList::execute()
{
	return std::make_shared<USObject>(vec);
}