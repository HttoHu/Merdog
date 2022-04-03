#include "../includes/decl.hpp"
#include "../includes/unit.hpp"
#include "../includes/memory.hpp"
#include "../includes/type.hpp"
#include "../includes/environment.hpp"
#include "../includes/lexer.hpp"
#include "../includes/unary_op.hpp"
#include <tuple>

namespace Mer {
	VarDecl::VarDecl(const NameT& _ids, const PartT& parts, size_t* _base_ptr, size_t _pos)
		:ParserNode(NodeType::VAR_DECL), ids(_ids), values(parts), base_ptr(_base_ptr), pos(_pos)
	{
	}
	void VarDecl::execute(char* dat)
	{
		dat = Mem::default_mem.buf+(*base_ptr + pos);

		for (auto [len, node] : values)
		{
			node->execute(dat);
			dat += len;
		}
	}

	std::string VarDecl::to_string() const
	{
		std::string ret=  "(let ";
		for (size_t i = 0; i < ids.size(); i++)
		{
			ret += "(" + ids[i] + " " + values[i].second->to_string() + ") ";
		}
		ret += ")";
		return ret;
	}

	namespace Parser {
		ParserNode* parse_expr();
		// type var1=expr1,var2=expr2.... => var_decl unit is var = expr..
		std::tuple<std::string, size_t, ParserNode*> parse_var_decl_unit(type_code_index type) {
			auto tok = token_stream.this_token();
			token_stream.next();
			if (tok->get_tag() != ID)
				throw Error("expect an ID token but got a "+tok->to_string());
			std::string var_name = Id::get_value(tok);

			if (token_stream.this_tag() == ASSIGN)
			{
				token_stream.match(ASSIGN);
				auto expr = parse_expr();

				if (expr->get_type() != type) {
					expr = new CastOp(expr, type);
				}
				return { var_name,Mer::get_type_size(type) ,expr };
			}
			throw Error("uninit var "+var_name);
		}
		void parse_const_decl_unit(type_code_index type) {
			auto tok = token_stream.this_token();
			token_stream.next();
			if (tok->get_tag() != ID)
				throw Error("expect an ID token but got a " + tok->to_string());
			std::string var_name = Id::get_value(tok);

			if (token_stream.this_tag() == ASSIGN)
			{
				token_stream.match(ASSIGN);
				auto expr = parse_expr();

				if (expr->get_type() != type) {
					expr = new CastOp(expr, type);
				}
				if (!expr->constant())
					throw Error("const symbol must can be caculated before runtime!");
				expr->execute(Mem::default_mem.buf);
				delete expr;
				expr = new LConV(Mem::default_mem.buf, type);
				Env::symbol_table.push_word(var_name, new Symbol::ConstRecorder(type, expr));
				return;
			}
			throw Error("uninit const " + var_name);
		}
		ParserNode* parse_var_decl()
		{
			type_code_index var_type = Parser::parse_type();
			size_t cur_pos = Mem::default_mem.var_idx;
			std::vector<std::pair<size_t, ParserNode*>> nodes;
			std::vector<std::string> ids;

			auto cur = parse_var_decl_unit(var_type);

			auto register_var_decl_unit = [&]() {
				std::string var_name = std::get<0>(cur);
				size_t var_size = std::get<1>(cur);

				ids.push_back(var_name);
				nodes.push_back({ var_size,std::get<2>(cur) });
				Env::symbol_table.push_word(var_name, new Symbol::VarRecorder(false, Mem::default_mem.var_idx,var_type));
				Mem::default_mem.push_var(var_size);
			};
			// push info
			register_var_decl_unit();

			while (token_stream.this_tag() == COMMA)
			{
				token_stream.match(COMMA);
				cur = parse_var_decl_unit(var_type);
				register_var_decl_unit();
			}
			return new VarDecl(ids, nodes, &Mem::default_mem.sp, cur_pos);
		}

		void parse_const_decl()
		{
			token_stream.match(CONST_DECL);
			type_code_index var_type = Parser::parse_type();
			parse_const_decl_unit(var_type);
			while (token_stream.this_tag() == COMMA)
			{
				token_stream.match(COMMA);
				parse_const_decl_unit(var_type);
			}
		}
	}
}


