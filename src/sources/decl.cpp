#include "../includes/decl.hpp"
#include "../includes/unit.hpp"
#include "../includes/memory.hpp"
#include "../includes/type.hpp"
#include "../includes/environment.hpp"
#include "../includes/lexer.hpp"
#include "../includes/unary_op.hpp"
#include <tuple>

namespace Mer
{
	VarDecl::VarDecl(const NameT& _ids, const PartT& parts, size_t* _base_ptr, size_t _pos)
		: ParserNode(NodeType::VAR_DECL), ids(_ids), values(parts), base_ptr(_base_ptr), pos(_pos)
	{
	}
	void VarDecl::execute(char* dat)
	{
		dat = Mem::default_mem.buf + (*base_ptr + pos);

		for (auto [len, node] : values)
		{
			node->execute(dat);
			dat += len;
		}
	}

	std::string VarDecl::to_string() const
	{
		std::string ret = "(let ";
		for (size_t i = 0; i < ids.size(); i++)
		{
			ret += "(" + ids[i] + " " + values[i].second->to_string() + ") ";
		}
		ret += ")";
		return ret;
	}
	NamePart::NamePart()
	{
		if (token_stream.this_tag() == MUL)
		{
			pointer = true;
			token_stream.match(MUL);
		}
		id = token_stream.this_token();
		token_stream.match(ID);
		while (token_stream.this_tag() == LSB)
		{
			arr = true;
			token_stream.match(LSB);
			if (token_stream.this_tag() == RSB)
			{
				if (auto_array)
					throw Error("you can not define two dimension auto array");

				token_stream.match(RSB);
				auto_array = true;
				return;
			}
			// get element count
			int ele_count = Utils::get_int_const_expr_value();
			count *= ele_count;
			array_indexs.push_back(ele_count);
			token_stream.match(RSB);
		}
		// if it is multidimensional array, count the size it needs to reserve
		if (array_indexs.size() > 1)
		{
			int f = array_indexs[0];
			for (int i = 1; i < array_indexs.size(); i++)
			{
				f *= array_indexs[i];
			}
			count = f + 1;
		}
	}

	namespace Parser
	{
		ParserNode* parse_expr();
		// type var1=expr1,var2=expr2.... => var_decl unit is var = expr..
		std::tuple<std::string,size_t, ParserNode*> parse_var_decl_unit(type_code_index type)
		{
			NamePart name_part;
			std::string var_name = Id::get_value(name_part.get_id());

			if (name_part.is_array())
			{
				auto length = name_part.get_array_element_cnt() * get_type_size(type);
				auto pos = Mem::default_mem.push_var(length);
				auto recorder = new Symbol::ArrayRecorder(type, name_part.array_indexs, pos, false);
				Env::symbol_table->push_word(var_name, recorder);
				return { var_name,length,new NonOp() };
			}
			else
			{
				if (token_stream.this_tag() == ASSIGN)
				{
					token_stream.match(ASSIGN);
					auto expr = parse_expr();

					if (expr->get_type() != type)
						expr = new CastOp(expr, type);
					auto pos = Mem::default_mem.push_var(get_type_size(type));
					Env::symbol_table->push_word(var_name, new Symbol::VarRecorder(false, 
						pos, type));

					return {var_name,get_type_size(type), expr };
				}
				throw Error("invalid decl " + var_name);
			}
		}
		void parse_const_decl_unit(type_code_index type)
		{
			auto tok = token_stream.this_token();
			token_stream.next();
			if (tok->get_tag() != ID)
				throw Error("expect an ID token but got a " + tok->to_string());
			std::string var_name = Id::get_value(tok);

			if (token_stream.this_tag() == ASSIGN)
			{
				token_stream.match(ASSIGN);
				auto expr = parse_expr();

				if (expr->get_type() != type)
				{
					expr = new CastOp(expr, type);
				}
				if (!expr->constant())
					throw Error("const symbol must can be caculated before runtime!");
				expr->execute(Mem::default_mem.buf);
				delete expr;
				expr = new LConV(Mem::default_mem.buf, type);
				Env::symbol_table->push_word(var_name, new Symbol::ConstRecorder(type, expr));
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

			auto register_var_decl_unit = [&]()
			{
				std::string var_name = std::get<0>(cur);
				size_t var_size = std::get<1>(cur);

				ids.push_back(var_name);
				nodes.push_back({ var_size, std::get<2>(cur) });
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
		ParserNode* statement()
		{
			ParserNode* node = nullptr;
			switch (token_stream.this_token()->get_tag())
			{
			case CHAR_DECL:
			case BOOL_DECL:
			case INTEGER_DECL:
			case REAL_DECL:
			case STRING_DECL:
				node = parse_var_decl();
				break;
			default:
				node = parse_expr();
				break;
			}
			token_stream.match(SEMI);
			return node;
		}
	}
}
