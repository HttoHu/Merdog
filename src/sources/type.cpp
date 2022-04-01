#include "../includes/type.hpp"
#include "../includes/defs.hpp"
#include "../includes/lexer.hpp"

namespace Mer {
	
	std::vector<Type*> type_tab = {
		new BasicType("void",0,{}),// 0 void,
		new BasicType("int",sizeof(int_default),{1,2,3}), // 1 int,
		new BasicType("real",sizeof(real_default),{1,2,3}), //2 real,
		new BasicType("byte",sizeof(byte_default),{1,2,3}), // 3 byte
	};
	namespace Parser {

		type_code_index parse_type()
		{
			auto tok = token_stream.this_token();
			token_stream.next();
			switch (tok->get_tag())
			{
			case INTEGER_DECL:
				return (type_code_index)BasicTypeTag::INT;
			case CHAR_DECL:
				return (type_code_index)BasicTypeTag::BYTE;
			case REAL_DECL:
				return (type_code_index)BasicTypeTag::REAL;
			default:
				throw Error("unsupported type!");
			}
		}
	}
	size_t get_type_size(type_code_index ty)
	{
		return type_tab[ty]->type_length();
	}
	type_code_index set_type(Type* ty)
	{
		type_tab.push_back(ty);
		return type_tab.size() - 1;
	}
}