/*function
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/parser.hpp"
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
#include "../include/function.hpp"
#include "../include/environment.hpp"
#include "../include/compound_box.hpp"
#include "../include/branch_and_loop.hpp"
using namespace Mer;
std::vector<ParserNode*> Mer::pre_stmt;
namespace Mer
{
	class For;
	class If;
	class Block;
	namespace Parser
	{
		ParserNode* statement();
	}
	Mem::Object Program::execute()
	{
		for (auto a : pre_stmt)
		{
			a->execute();
		}
		for (*pc = 0; *pc < stmts.size(); ++ * pc)
			stmts[*pc]->execute();
		return nullptr;
	}
	std::string Program::to_string()
	{
		return identify->to_string();
	}
	bool & global_stmt()
	{
		static bool ret = true;
		return ret;
	}
}
Program* Mer::Parser::program()
{
	Program* ret = nullptr;
	int programe_num = 0;
	while (1)
	{
		switch (token_stream.this_tag())
		{
		case STRUCT:
			build_ustructure();
			break;
		case FUNCTION:
			build_function();
			break;
		case PROGRAM:
		{
			global_stmt() = false;
			token_stream.match(PROGRAM);
			auto tmp = token_stream.this_token();
			token_stream.match(ID);
			std::string name = Id::get_value(tmp);
			ret = new Program(tmp);
			_pcs.push_back(ret->pc);
			current_ins_table = &(ret->stmts);
			Parser::build_block();
			programe_num++;
			break;
		}
		case ENDOF:
			if (programe_num != 1)
				throw Error("The program must have a program as an entry");
			return ret;
		case ID:
		default:
			pre_stmt.push_back(Parser::statement());
			break;
		}
	}

}

ParserNode* Mer::Parser::statement()
{
	ParserNode* node = nullptr;
	switch (token_stream.this_token()->get_tag())
	{
	case BOOL_DECL:
	case INTEGER_DECL:
	case REAL_DECL:
	case STRING_DECL:
		node = var_decl();
		break;
	default:
		node = new Expr();
		break;
	}
	token_stream.match(SEMI);
	return node;
}


ParserNode* Mer::Parser::var_decl()
{
	size_t type = Mem::get_type_code();
	std::vector<VarDeclUnit*> units;
	units.push_back(new VarDeclUnit(type));
	while (token_stream.this_tag() != SEMI)
	{
		token_stream.match(COMMA);
		units.push_back(new VarDeclUnit(type));
	}
	if (global_stmt())
	{
		return new GloVarDecl(units, type);
	}
	return new LocalVarDecl(units, type);
}

size_t Mer::Parser::get_type()
{
	switch (token_stream.this_tag())
	{
	case VOID_DECL:
		token_stream.match(VOID_DECL);
		return Mem::BVOID;
	case INTEGER_DECL:
		token_stream.match(INTEGER_DECL);
		return Mem::INT;
	case REAL_DECL:
		token_stream.match(REAL_DECL);
		return Mem::DOUBLE;
	case BOOL_DECL:
		token_stream.match(BOOL_DECL);
		return Mem::BOOL;
	case STRING_DECL:
		token_stream.match(STRING_DECL);
		return Mem::STRING;
	case ID:
	{
		auto info = Mer::this_namespace->sl_table->find(Id::get_value(token_stream.this_token()));
		if (info == nullptr)
			throw Error("id: " + Id::get_value(token_stream.this_token()) + "no found");
		token_stream.next();
		return info->get_type();
	}
	default:
		throw Error("not finished yet");
	}
}

WordRecorder* Mer::Parser::get_current_info()
{
	return this_namespace->sl_table->find(Mer::Id::get_value(token_stream.this_token()));
}



Mer::NamePart::NamePart()
{
	if (token_stream.this_tag() == MUL)
	{
		pointer = true;
		token_stream.match(MUL);
	}
	id = token_stream.this_token();
	token_stream.match(ID);
	if (token_stream.this_tag() == LSB)
	{
		arr = true;
		token_stream.match(LSB);
		if (token_stream.this_tag() == RSB)
		{
			token_stream.match(RSB);
			auto_array = true;
			return;
		}
		auto c = token_stream.this_token();
		count = Integer::get_value(c);
		token_stream.match(INTEGER);
		token_stream.match(RSB);
	}

}

Mer::VarDeclUnit::VarDeclUnit(size_t t) :type_code(t)
{
	NamePart name_part;
	id = name_part.get_id();
	is_arr = name_part.is_array();
	if (name_part.is_pointer())
	{
		is_p = true;
		type_code++;
	}
	// manage to process array 
	if (name_part.is_array())
	{
		// if the array's size is deduced by init_list.
		if (name_part.is_auto_array())
		{
			token_stream.match(ASSIGN);
			// -1 represent an auto init_list;
			auto initl= new InitList(type_code, -1);
			size = initl->get_ele_count();
			expr = initl;
		}
		else
		{
			size = name_part.get_count();
			if (token_stream.this_tag() == ASSIGN)
			{
				token_stream.match(ASSIGN);
				expr = new InitList(type_code, name_part.get_count());
			}
			else {
				expr = new EmptyList(type_code, name_part.get_count());
			}
		}
		return;
	}
	if (type_name_mapping.find(type_code) != type_name_mapping.end())
	{
		UStructure* result = find_ustructure_t(type_code);
		if (token_stream.this_tag() == BEGIN)
			expr = new StructureInitList(result->mapping);
		else if (token_stream.this_tag() == ASSIGN)
		{
			goto tt;
		}
		else
		{
			expr = new DefaultInitList(type_code);
		}
		return;
	}
	if (token_stream.this_tag() == ASSIGN)
	{
tt:		token_stream.match(ASSIGN);
		expr = (new Expr(type_code))->root();
		if (type_code != expr->get_type())
			throw Error("::VarDeclUnit::VarDeclUnit(size_t t): type not matched, from " + std::to_string(type_code) + " to " + std::to_string(expr->get_type()));
		return;
	}

	throw Error("::VarDeclUnit::VarDeclUnit(size_t t) : try to init a non-init variable");
}
inline void _record_id(Mer::VarDeclUnit *var_unit, size_t type,size_t pos)
{
	if(var_unit->arr())
		this_namespace->sl_table->push(Id::get_value(var_unit->get_id()), new VarIdRecorder(type , pos, ESymbol::SARRAY));
	else if (var_unit->pointer())
		this_namespace->sl_table->push(Id::get_value(var_unit->get_id()), new VarIdRecorder(type+1, pos));
	else
		this_namespace->sl_table->push(Id::get_value(var_unit->get_id()), new VarIdRecorder(type, pos));
}
inline void _record_glo_id(Mer::VarDeclUnit* var_unit, size_t type, size_t pos)
{
	if (var_unit->arr())
		this_namespace->sl_table->push(Id::get_value(var_unit->get_id()), new GVarIdRecorder(type, pos, ESymbol::SARRAY));
	else if (var_unit->pointer())
		this_namespace->sl_table->push(Id::get_value(var_unit->get_id()), new GVarIdRecorder(type+1, pos));
	else
		this_namespace->sl_table->push(Id::get_value(var_unit->get_id()), new GVarIdRecorder(type, pos));
}
Mer::LocalVarDecl::LocalVarDecl(std::vector<VarDeclUnit*>& vec, size_t t) :type(t)
{
	for (const auto& a : vec)
	{
		sum += a->get_size();
	}
	pos = mem.push(sum) - sum;
	size_t tmp_pos = pos;
	// the var may be array ,pointer or a common var.
	_record_id(vec[0], type, pos);
	for (size_t i = 1; i < vec.size(); i++)
	{
		_record_id(vec[i], type, tmp_pos += vec[i - 1]->get_size());
	}
	for (auto& a : vec)
	{
		if (a->arr())
		{
			std::vector<Expr*> arr;
			auto exprs_info = a->get_expr();
			if (typeid(*exprs_info) == typeid(InitList))
				arr = static_cast<InitList*>(a->get_expr())->exprs();
			else
				arr = static_cast<EmptyList*>(a->get_expr())->exprs();
			exprs.insert(exprs.end(), arr.begin(), arr.end());
		}
		else {
			exprs.push_back(static_cast<Expr*>(a->get_expr()));
		}
	}
}

Mem::Object Mer::LocalVarDecl::execute()
{
	for (int i = 0; i < sum; i++) {
		mem[mem.get_current() + pos + i] = exprs[i]->execute()->clone();
	}
	return Mem::Object(nullptr);
}

Mer::GloVarDecl::GloVarDecl(std::vector<VarDeclUnit*>& vec, size_t t) :type(t)
{
	for (const auto& a : vec)
	{
		sum += a->get_size();
	}
	pos = mem.push(sum) - sum;
	size_t tmp_pos = pos;
	_record_glo_id(vec[0], type, pos);
	for (size_t i = 1; i < vec.size(); i++)
		_record_glo_id(vec[i], type, tmp_pos += vec[i - 1]->get_size());
	for (auto& a : vec)
	{
		if (a->arr())
		{
			auto arr = static_cast<InitList*>(a->get_expr())->exprs();
			exprs.insert(exprs.end(), arr.begin(), arr.end());
		}
		else {
			exprs.push_back(static_cast<Expr*>(a->get_expr()));
		}
	}
}

Mem::Object Mer::GloVarDecl::execute()
{
	for (size_t i = 0; i < sum; i++) {
		mem[pos + i] = exprs[i]->execute();
	}
	return Mem::Object(nullptr);
}

Mer::Cast::Cast()
{
	token_stream.match(CAST);
	token_stream.match(LT);
	to_type = Mem::get_type_code(token_stream.this_token());
	token_stream.next();
	token_stream.match(GT);
	token_stream.match(LPAREN);
	expr = new Expr();
	token_stream.match(RPAREN);
}

Mem::Object Mer::Cast::execute()
{
	return expr->execute()->Convert(to_type);
}
