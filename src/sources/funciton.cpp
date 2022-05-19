#pragma once
#include "../includes/lexer.hpp"
#include "../includes/function.hpp"
#include "../includes/type.hpp"
#include "../includes/memory.hpp"
#include "../includes/decl.hpp"
#include "../includes/environment.hpp"
#include "../includes/branch_loop.hpp"


namespace Mer {
	bool Param::type_check(const std::vector<type_code_index>& types)
	{
		if (types.size() != arg_pos.size())
			return false;
		for (size_t i = 0; i < types.size(); i++)
		{
			if (types[i] >= type_tab.size())
				throw Error("<inner error> type were not found!");
			auto type_seeker = type_tab[types[i]];
			if (!type_seeker->convertible(arg_pos[i].second))
				return false;
		}
		return true;
	}

	ParamFeature Param::get_param_feature()
	{
		ParamFeature ret;
		for (auto a : arg_pos)
		{
			ret.push_back(a.first);
		}
		return ret;
	}

	Function::Function(type_code_index t, Param* p) :type(t) {
		param_types = p->get_param_feature();
	}

	Function::Function(type_code_index t) : type(t) {}

	void Function::run(char* mem)
	{
		Mem::default_mem.new_func(off);
		size_t tmp = *pc;
		for (*pc = 0; *pc < stmts.size(); ++ * pc) {
			stmts[*pc]->execute(mem);
		}
		*pc = tmp;
		Mem::default_mem.end_func();
	}

	namespace Parser {
		Param* parse_parameter() {
			Param* ret = new Param();
			token_stream.match(LPAREN);
			while (token_stream.this_tag() != RPAREN)
			{
				auto type = Parser::parse_type();
				auto name = new NamePart();

				size_t pos = Mem::default_mem.push_var(name->get_count());
				Env::symbol_table->push_word(Id::get_value(name->get_id()), new Symbol::VarRecorder(false, pos, type));

				ret->push_new_param(type, pos);
				delete name;
				if (token_stream.this_tag() == COMMA)
					token_stream.match(COMMA);
				else
					break;
			}
			token_stream.match(RPAREN);
			return ret;
		}

		void parse_function()
		{
			token_stream.match(FUNCTION);

			std::string name = Id::get_value(token_stream.this_token());
			token_stream.next();
			Mem::default_mem.new_block();

			auto params = parse_parameter();

			token_stream.match(ARROW);
			

		}
	}
}