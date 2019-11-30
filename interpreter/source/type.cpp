/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/lexer.hpp"
#include "../include/namespace.hpp"
#include "../include/word_record.hpp"
#include "../include/type.hpp"
#include "../include/error.hpp"
namespace Mer
{
	extern Namespace* this_namespace;

	namespace Mem
	{
		//var_def
		const std::map<type_code_index, Mem::Type*> _init_type_map{
			{ BasicType::INT,new Type("int",BasicType::INT,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE })},
			{ BasicType::DOUBLE,new Type("double",BasicType::DOUBLE,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE }) },
			{ BasicType::BOOL,new Type("bool",BasicType::BOOL,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE }) },
			{ BasicType::STRING,new Type("string",BasicType::STRING,{ 11,BasicType::STRING }) },
			{BasicType::CHAR,new Type("char",BasicType::CHAR,{9,BasicType::CHAR,BasicType::INT,BasicType::STRING})},
			{BasicType::INIT_LIST,new Type("init_list",BasicType::INIT_LIST,{})}
		};
		type_code_index type_counter = BASICTYPE_MAX_CODE;
		std::map<type_code_index, void(*)(type_code_index)> container_register;
		std::map<ComplexType, type_code_index> complex_type_seeker;
		std::map<type_code_index, ComplexType> demerge_table;
		std::map<ComplexType, type_code_index> merge_table;
		std::map<type_code_index, std::map<std::string, type_code_index>> type_op_type_map
		{
			{Mem::STRING,{{"[]",Mem::CHAR}}}
		};
		std::map<type_code_index, Mem::Type*> type_map = _init_type_map;
		//===============================================================================================
		type_code_index find_op_type(type_code_index ty, std::string op)
		{
			auto result1 = type_op_type_map.find(ty);
			if (result1 == type_op_type_map.end())
				throw Error("type " + std::to_string(ty) + " hasn't registerd the operator " + op);
			auto result2 = result1->second.find(op);
			if (result2 == result1->second.end())
				throw Error("type " + std::to_string(ty) + " doesn't support the operator " + op);
			return result2->second;
		}

		bool exist_operator(type_code_index ty, std::string op)
		{
			auto result1 = type_op_type_map.find(ty);
			if (result1 == type_op_type_map.end())
				return false;
			auto result2 = result1->second.find(op);
			if (result2 == result1->second.end())
				return false;
			return true;
		}

		type_code_index get_type_code()
		{
			auto tok = token_stream.this_token();
			token_stream.next();
			switch (tok->get_tag())
			{
			case CHAR_DECL:
				return CHAR;
			case VOID_DECL:
				return BVOID;
			case INTEGER_DECL:
				return INT;
			case REAL_DECL:
				return DOUBLE;
			case BOOL_DECL:
				return BOOL;
			case STRING_DECL:
				return STRING;
			case ID:
			{
				auto info = Mer::this_namespace->sl_table->find(Id::get_value(tok));
				if (info == nullptr)
					throw Error("type: " + Id::get_value(tok) + " no found");
				if (info->es == ESymbol::SCONTAINER)
				{
					token_stream.back();
					return get_ctype_code();
				}
				if (info == nullptr)
					throw Error("id: " + Id::get_value(token_stream.this_token()) + "no found");
				return info->get_type();
			}
			default:
				throw Error(token_stream.this_token()->to_string() + " unknown type ");
			}
		}

		type_code_index get_type_code(Token* tok)
		{
			switch (tok->get_tag())
			{
			case CHAR_DECL:
				return CHAR;
			case VOID_DECL:
				return BVOID;
			case INTEGER_DECL:
				return INT;
			case REAL_DECL:
				return DOUBLE;
			case BOOL_DECL:
				return BOOL;
			case STRING_DECL:
				return STRING;
			case ID:
			{
				auto info = Mer::this_namespace->sl_table->find(Id::get_value(token_stream.this_token()));
				if (info == nullptr)
					throw Error("id: " + Id::get_value(token_stream.this_token()) + "no found");
				return info->get_type();

			}
			default:
				throw Error(tok->to_string() + " unknown type ");
			}
		}

		type_code_index& type_no()
		{
			static type_code_index this_type_index = 10;
			return this_type_index;
		}
		// to DO 
		type_code_index get_ctype_code()
		{
			type_code_index container_type;
			type_code_index element_type;
			container_type = Parser::get_type();
			if (token_stream.this_tag() != LT)
			{
				throw Error("uncomplete type cann't be used");
			}
			token_stream.match(LT);
			element_type = get_type_code();
			// get pair map<string,int> m;
			if (token_stream.this_tag() == COMMA)
			{
				token_stream.match(COMMA);
				type_code_index secondary_type = get_type_code();
				Mem::type_counter += 2;
				int type_c = Mem::type_counter;

				demerge_table.insert({ type_c,ComplexType{element_type,secondary_type} });
				merge_table.insert({ ComplexType{ element_type, secondary_type}, type_c });
				token_stream.match(GT);
				return register_container(container_type, type_c);
			}
			token_stream.match(GT);
			return merge(container_type, element_type);
		}

		std::pair<type_code_index, type_code_index> demerge(type_code_index t)
		{
			auto result = demerge_table.find(t);
			if (result == demerge_table.end())
				throw Error("type " + std::to_string(t) + " can not split");
			return { result->second.container_type,result->second.element_type };
		}

		int register_container(type_code_index container_type, type_code_index element_type)
		{
			Mem::type_counter += 2;
			int type_c = Mem::type_counter;
			demerge_table.insert({ type_c,ComplexType{container_type,element_type} });
			merge_table.insert({ ComplexType{ container_type,element_type }, type_c });
			auto result = container_register.find(container_type);
			if (result == container_register.end())
			{
				throw Error("intern error!");
			}
			(*result->second)(element_type);
			return type_c;
		}

		type_code_index merge(type_code_index l, type_code_index r)
		{
			auto result = merge_table.find(ComplexType{ l,r });
			if (result == merge_table.end())
			{

				return register_container(l, r);

			}
			return result->second;
		}

		void _clear_type_info()
		{
			container_register.clear();
			complex_type_seeker.clear();
			demerge_table.clear();
			merge_table.clear();
			type_op_type_map = {
				{Mem::STRING,{{"[]",Mem::CHAR}}}
			};
			for (auto &a : type_map)
			{
				if (_init_type_map.find(a.first) != _init_type_map.end())
					continue;
				delete a.second;
			}
			type_map = _init_type_map;
			type_counter = BASICTYPE_MAX_CODE;
		}
	}
	std::map<std::string, type_code_index> Mem::type_index;
}

