#include "../include/compound_box.hpp"
#include "../include/lexer.hpp"
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
using namespace Mer;
std::map<std::string, Mer::UStructrue*> Mer::ustructure_map;
std::map<std::string, size_t> Mer::name_type_mapping;
std::map<size_t,std::string> Mer::type_name_mapping;
//OK
void Mer::build_ustructure()
{
	token_stream.match(STRUCT);
	std::string name = Id::get_value(token_stream.this_token());
	token_stream.match(ID);
	token_stream.match(BEGIN);
	UStructrue* us=new UStructrue();
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
	name_type_mapping.insert({ name,++Mem::type_counter });
	type_name_mapping.insert({ Mem::type_counter,name });
	Mer::this_namespace->sl_table->push(name, new WordRecorder(ESymbol::SSTRUCTURE));
}

StructureDecl* Mer::structure_decl()
{
	std::vector<Token*> tvec;
	std::string type_name = Id::get_value(token_stream.this_token());
	token_stream.next();
	auto result = ustructure_map.find(type_name);
	if (result == ustructure_map.end())
	{
		throw Error("undefined type <" + type_name + ">");
	}
	auto type_result = name_type_mapping.find(type_name);
	tvec.push_back(token_stream.this_token());
	token_stream.match(ID);
	while (token_stream.this_tag() != SEMI)
	{
		token_stream.match(COMMA);
		tvec.push_back(token_stream.this_token());
		token_stream.match(ID);
	}
	return new StructureDecl(type_result->second,tvec);
}

//OK
Mer::UStructrue* Mer::find_ustructure_t(size_t type)
{
	auto result = type_name_mapping.find(type);
	if (result == type_name_mapping.end())
		throw Error("type " + std::to_string(type) + " undefined");
	auto result2 = ustructure_map.find(result->second);
	if (result2 == ustructure_map.end())
		throw Error("Id " +result2->first + " undefined");
	return result2->second;
}
//OK
std::vector<Mem::Object> Mer::UStructrue::init()
{
	std::vector<Mem::Object> ret;
	for (const auto &a : STMapping)
	{
		ret.push_back(Mem::create_var_t(a.second));
	}
	return ret;
}

Mer::StructureDecl::StructureDecl(size_t t, const std::vector<Token*>& v)
{
	type = t;
	for (auto& a : v)
	{
		auto pos = stack_memory.push();
		this_namespace->sl_table->push(Id::get_value(a), new VarIdRecorder(t, pos));
		var_list.push_back({ pos });
	}
}

Mem::Object Mer::StructureDecl::execute()
{
	auto tmp = find_ustructure_t(type);
	for (auto& a : var_list)
	{
		stack_memory[a] = std::make_shared<USObject>(tmp->init());
	}
	return nullptr;
}
