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
	Mem::type_index.insert({ name,++Mem::type_counter });
	type_name_mapping.insert({ Mem::type_counter,name });
	Mer::this_namespace->sl_table->push(name, new WordRecorder(ESymbol::SSTRUCTURE,Mem::type_counter));
	Mem::type_map.insert({ Mem::type_counter ,new Mem::Type(name,Mem::type_counter,{size_t(Mem::type_counter)}) });
}

StructureDecl* Mer::structobj_decl()
{
	std::vector<std::pair<Token*,SInitBase*>> tvec;
	std::string type_name = Id::get_value(token_stream.this_token());
	token_stream.next();
	auto result = ustructure_map.find(type_name);
	if (result == ustructure_map.end())
	{
		throw Error("undefined type <" + type_name + ">");
	}
	auto type_result = Mem::type_index.find(type_name);
	Token* first_id=token_stream.this_token();
	token_stream.match(ID);
	if (token_stream.this_tag() == BEGIN)
	{
		auto init_list = new StructureInitList(result->second->mapping);
		tvec.push_back({ first_id,init_list });
	}
	else
		tvec.push_back({ first_id,new StructureEmptyInit(result->second) });
	while (token_stream.this_tag() != SEMI)
	{
		token_stream.match(COMMA);
		Token *remain_ids = token_stream.this_token();
		token_stream.match(ID);
		if (token_stream.this_tag() == BEGIN)
		{
			tvec.push_back({ remain_ids,new StructureInitList(result->second->mapping) });
		}
		else
			tvec.push_back({ remain_ids,new StructureEmptyInit(result->second) });
	}
	return new StructureDecl(type_result->second,tvec);
}

//OK
Mer::UStructure* Mer::find_ustructure_t(size_t type)
{
	auto result = type_name_mapping.find(type);
	if (result == type_name_mapping.end())
		throw Error("type" + std::to_string(type) + " undefined");
	auto result2 = ustructure_map.find(result->second);
	if (result2 == ustructure_map.end())
		throw Error("Id " +result2->first + " undefined");
	return result2->second;
}
//OK
std::vector<Mem::Object> Mer::UStructure::init()
{
	std::vector<Mem::Object> ret;
	for (const auto &a : STMapping)
	{
		ret.push_back(Mem::create_var_t(a.second));
	}
	return ret;
}

Mer::StructureDecl::StructureDecl(size_t t, const std::vector<std::pair<Token*, SInitBase*>>& v)
{
	type = t;
	for (auto& a : v)
	{
		auto pos = stack_memory.push();
		this_namespace->sl_table->push(Id::get_value(a.first), new VarIdRecorder(t, pos));
		var_list.push_back({ pos,a.second });
	}
}

Mem::Object Mer::StructureDecl::execute()
{
	auto tmp = find_ustructure_t(type);
	for (auto& a : var_list)
	{
		stack_memory[a.first] = a.second->transfer();
	}
	return nullptr;
}

std::shared_ptr<USObject> Mer::StructureInitList::transfer()
{
	std::vector<Mem::Object> obj_vec(vec.size());
	for (int i=0;i<vec.size();i++)
	{
		obj_vec[i] = vec[i]->execute();
	}
	return std::make_shared<USObject>(obj_vec);
}

Mer::StructureInitList::StructureInitList(const std::map<std::string, int>& m):vec(m.size())
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
