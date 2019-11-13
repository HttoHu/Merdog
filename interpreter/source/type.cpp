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
		int type_counter = 9;
		std::map<size_t, void(*)(size_t)> container_register;
		std::map<ComplexType, size_t> complex_type_seeker;
		std::map<size_t, ComplexType> demerge_table;
		std::map<ComplexType, size_t> merge_table;
		std::map<size_t, std::map<std::string, size_t>> type_op_type_map
		{
			{Mem::STRING,{{"[]",Mem::CHAR}}}
		};
		std::map<size_t, Mem::Type*> type_map
		{
			{ BasicType::INT,new Type("int",BasicType::INT,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE })},
			{ BasicType::DOUBLE,new Type("double",BasicType::DOUBLE,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE }) },
			{ BasicType::BOOL,new Type("bool",BasicType::BOOL,{ BasicType::INT,BasicType::BOOL,BasicType::DOUBLE }) },
			{ BasicType::STRING,new Type("string",BasicType::STRING,{ 11,BasicType::STRING }) },
			{BasicType::CHAR,new Type("char",BasicType::CHAR,{9,BasicType::CHAR,BasicType::INT,BasicType::STRING})}
		};
		size_t find_op_type(size_t ty, std::string op)
		{
			auto result1 = type_op_type_map.find(ty);
			if (result1 == type_op_type_map.end())
				throw Error("type " + std::to_string(ty) + " hasn't registerd the operator " + op);
			auto result2 = result1->second.find(op);
			if (result2 == result1->second.end())
				throw Error("type " + std::to_string(ty) + " doesn't support the operator " + op);
			return result2->second;
		}

		bool exist_operator(size_t ty, std::string op)
		{
			auto result1 = type_op_type_map.find(ty);
			if (result1 == type_op_type_map.end())
				return false;
			auto result2 = result1->second.find(op);
			if (result2 == result1->second.end())
				return false;
			return true;
		}
		size_t Mer::Mem::get_type_code()
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
					throw Error("id: " + Id::get_value(token_stream.this_token()) + "no found");
				return info->get_type();
			}
			default:
				throw Error(token_stream.this_token()->to_string() + " unknown type ");
			}
		}

		size_t Mer::Mem::get_type_code(Token* tok)
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

		size_t& Mer::Mem::type_no()
		{
			static size_t this_type_index = 10;
			return this_type_index;
		}
		// to DO 
		size_t Mer::Mem::get_ctype_code()
		{
			size_t container_type;
			size_t element_type;
			container_type = Parser::get_type();
			if (token_stream.this_tag() != LT)
			{
				return container_type;
			}
			token_stream.match(LT);
			element_type = get_ctype_code();
			token_stream.match(GT);
			return merge(container_type, element_type);
		}

		int regitser_container(size_t container_type, size_t element_type)
		{
			int type_c = Mem::type_counter += 2;
			demerge_table.insert({ type_c,ComplexType{container_type,element_type} });
			merge_table.insert({ ComplexType{ container_type,element_type }, type_c });
			auto result = container_register.find(container_type);
			if (result == container_register.end())
			{
				throw Error("intern error!");
			}
			(*result->second)(element_type);
		}

		size_t merge(size_t l, size_t r)
		{
			auto result = merge_table.find(ComplexType{ l,r });
			if (result == merge_table.end())
			{
				return regitser_container(l, r);

			}
			return result->second;
		}
	}
	std::map<std::string, size_t> Mem::type_index;
}

