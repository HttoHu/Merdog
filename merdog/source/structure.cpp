#include "../include/structure.hpp"
#include "../include/lexer.hpp"
#include "../include/namespace.hpp"
namespace Mer
{
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
		Structure *ret = new Structure(structure_info);
		this_namespace->set_new_structure(sname, ret);
		return ret;
	}
}

