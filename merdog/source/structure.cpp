#include "../include/structure.hpp"
#include "../include/lexer.hpp"
#include "../include/namespace.hpp"
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
namespace Mer
{
	std::vector<Type*> type_vtable{
		new Type(1,"void",0),
		new Type(1,"int",1),new Type(1,"double",3),
		new Type(1,"string",5),new Type(1,"bool",7),
	};
	Structure * Parser::parse_structure()
	{
		std::map<std::string, std::pair<size_t, size_t>> structure_info;
		token_stream.match(STRUCT);
		std::string sname = GIC();
		//so nice U are.
		token_stream.match(BEGIN);
		size_t index = 0;
		while (token_stream.this_tag() != END)
		{
			auto type = Mem::get_type_code(token_stream.this_token());
			token_stream.next();
			auto id = GIC();
			structure_info.insert({ id,{index++,type} });
			token_stream.match(SEMI);
		}
		token_stream.match(END);
		token_stream.match(SEMI);
		Structure *ret = new Structure(sname,structure_info);
		this_namespace->set_new_structure(sname, ret);
		return ret;
	}
	Mer::Type * Mer::Parser::find_struct(size_t _type)
	{
		if (_type < type_vtable.size())
		{
			return type_vtable[_type];
		}
		else
			throw Error("invaild type");
	}
}
