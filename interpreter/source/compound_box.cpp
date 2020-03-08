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
namespace Mer
{
	std::map<std::string, UStructure*> ustructure_map;
	std::map<type_code_index, std::string> type_name_mapping;
	std::map<type_code_index, std::map<std::string, FunctionBase*>> member_function_table;
	std::vector<Mem::Object> parents_vec;
	//OK
	void build_ustructure()
	{
		token_stream.match(STRUCT);
		std::string name = Id::get_value(token_stream.this_token());
		// you need to record the type info of the struct imediately, or you can't define the pointer of self-type;
		Mem::type_counter += 2;
		Mer::this_namespace->sl_table->push(name, new WordRecorder(ESymbol::SSTRUCTURE, Mem::type_counter));
		token_stream.match(ID);
		token_stream.match(BEGIN);
		UStructure* us = new UStructure();
		//register struct 
		ustructure_map.insert({ name,us });
		Mem::type_index.insert({ name,Mem::type_counter });
		type_name_mapping.insert({ Mem::type_counter,name });

		Mem::type_map.insert({ Mem::type_counter ,new Mem::Type(name,Mem::type_counter,{Mem::type_counter}) });
		tsymbol_table->new_block();
		while (token_stream.this_tag() != END)
		{
			// parse member function;
			if (token_stream.this_tag() == FUNCTION)
			{
				std::pair<std::string, Function*> tmp = Parser::_build_function();
				// push function 
				member_function_table[Mem::type_counter].insert(tmp); 
				// record function id
				auto recorder = new FuncIdRecorder(tmp.second);
				us->structure_member_table.insert({ tmp.first,recorder});
				this_namespace->sl_table->push(tmp.first, recorder);
				continue;
			}
			size_t type = Mem::get_type_code();
			// when its type is a pointer
			if (token_stream.this_tag() == MUL)
			{
				token_stream.match(MUL);
				type++;
			}
			// record
			std::string mem_name = Id::get_value(token_stream.this_token());
			us->push_new_children(type, mem_name);
			auto id_recorder = new MVarRecorder(type, (size_t)(us->be) - 1u);
			tsymbol_table->push(mem_name, id_recorder);
			us->structure_member_table.insert({ mem_name,id_recorder });

			token_stream.match(ID);
			// process init_value
			if (token_stream.this_tag() == ASSIGN)
			{
				token_stream.match(ASSIGN);
				auto expr = Expr(type).root();
				us->push_init(expr->execute());
				if (expr->get_type() != type)
				{
					delete expr;
					throw Error("struct member type is not matched with init value");
				}
				delete expr;
			}
			else
			{
				us->push_init(Mem::create_var_t(type));
			}
			token_stream.match(SEMI);
		}
		tsymbol_table->end_block();
		token_stream.match(END);

	}


	UStructure* find_ustructure_t(type_code_index type)
	{
		auto result = type_name_mapping.find(type);
		if (result == type_name_mapping.end())
			throw Error("struct type " + std::to_string(type) + " undefined");
		auto result2 = ustructure_map.find(result->second);
		if (result2 == ustructure_map.end())
			throw Error("Id " + result2->first + " undefined");
		return result2->second;
	}
	void UStructure::push_new_children(size_t t, std::string id_name)
	{
		mapping.insert({ id_name,be++ });
		STMapping.insert({ id_name,t });
	}
	std::pair<type_code_index, size_t> UStructure::get_member_info(std::string member_name)
	{
		auto result = mapping.find(member_name);
		if (result == mapping.end())
			throw Error("member " + member_name + " no found!");
		auto type_result = STMapping.find(member_name);
		return { type_result->second,result->second };
	}
	std::vector<Mem::Object> UStructure::init()
	{
		std::vector<Mem::Object> ret(init_vec.size());
		for (size_t i = 0; i < ret.size(); i++)
		{
			ret[i] = init_vec[i]->clone();
		}
		return ret;
	}

	void UStructure::print()
	{
		std::cout << "------Name-Type-Map------\n";
		for (auto a : STMapping)
		{
			std::cout << "name:" << a.first << "\t\t\t" << "type: " << a.second << std::endl;
		}
		std::cout << "------Name-Position_Map------\n";
		for (auto a : mapping)
		{
			std::cout << "name:" << a.first << "\t\t\t" << "position: " << a.second << std::endl;
		}
		std::cout << "-----------------------------------\n";
	}

	WordRecorder* UStructure::find_id_info(const std::string& id)
	{
		auto result = structure_member_table.find(id);
		if (result == structure_member_table.end())
			throw Error("member " + id + " no found!");
		return result->second;
	}



	Mem::Object StructureInitList::execute()
	{
		std::vector<Mem::Object> obj_vec(vec.size());
		for (size_t i = 0; i < vec.size(); i++)
		{
			obj_vec[i] = vec[i]->execute();
		}
		return std::make_shared<USObject>(obj_vec);
	}

	ParserNode* StructureInitList::clone()
	{
		auto ret = new StructureInitList;
		ret->type_code = type_code;
		for (auto a : vec)
			ret->vec.push_back(a->clone());
		return ret;
	}

	Mer::StructureInitList::StructureInitList(UStructure *us, type_code_index _type_code) : type_code(_type_code)
	{
		const auto& type_info_map = us->STMapping;
		const auto& str_pos_map = us->mapping;
		std::set<std::string> member_name_set;
		for (const auto& a : type_info_map)
			member_name_set.insert(a.first);
		vec.resize(type_info_map.size());
		token_stream.match(BEGIN);
		size_t last_index = -1;
		while (token_stream.this_tag() != END) {
			auto member_suffix = token_stream.this_token();
			auto result = us->get_member_info(Id::get_value(member_suffix));
			member_name_set.erase(Id::get_value(member_suffix));
			token_stream.match(ID);
			token_stream.match(COLON);
			vec[result.second] = Expr(result.first).root();
			if (token_stream.this_tag() == END)
			{
				token_stream.match(END);
				break;
			}
			token_stream.match(COMMA);
		}
		for (auto& name : member_name_set)
		{
			auto result = us->get_member_info(name);
			vec[result.second] = new LConV(Mem::create_var_t(result.first),result.first);
		}
	}

	Mer::DefaultInitList::DefaultInitList(type_code_index type)
	{
		auto ustruct = find_ustructure_t(type);
		vec = ustruct->init();
	}

	Mem::Object DefaultInitList::execute()
	{
		std::vector<Mem::Object> ret(vec.size());
		for (size_t i = 0; i < ret.size(); i++)
			ret[i] = vec[i]->clone();
		return std::make_shared<USObject>(ret);
	}

	ParserNode* DefaultInitList::clone()
	{
		auto ret = new DefaultInitList;
		ret->vec = vec;
		return ret;
	}

	Mem::Object USObject::operator=(Mem::Object v) {
		// I am trying to do as much as possible check before runtime
		auto a = v->clone();
		this->vec = std::static_pointer_cast<USObject>(v)->vec;
		return a;
	}

	Mem::Object USObject::clone() const
	{
		std::vector<Mem::Object> ret_vec(vec.size());
		for (size_t i = 0; i < ret_vec.size(); i++)
		{
			ret_vec[i] = vec[i]->clone();
		}
		return std::make_shared<USObject>(ret_vec);
	}

	std::string USObject::to_string() const
	{
		std::string ret = "";
		for (auto& a : vec)
		{
			ret += a->to_string() + "     ";
		}
		return ret;
	}
	Mem::Object MemberVar::execute()
	{
		return parents_vec.back()->operator[](std::make_shared<Mem::Int>(member_pos));
	}
}