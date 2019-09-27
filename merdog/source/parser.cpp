/*
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
#include "../include/loop_statement.hpp"
using namespace Mer;
bool Mer::global_stmt = true;
std::vector<ParserNode*> Mer::pre_stmt;
namespace Mer
{
	class For;
	class If;
	class Block;
	namespace Parser
	{
		ParserNode* while_statement();
		ParserNode* for_statement();
		ParserNode* if_statement();
		ParserNode* statement();
		Block* block();
		ParserNode* switch_statement();
	}
	Mem::Object Program::execute()
	{
		for (auto a : pre_stmt)
		{
			a->execute();
		}
		blo->execute();
		return nullptr;
	}
	std::string Program::to_string()
	{
		return identify->to_string();
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
			global_stmt = false;
			token_stream.match(PROGRAM);
			auto tmp = token_stream.this_token();
			token_stream.match(ID);
			std::string name = Id::get_value(tmp);
			auto blo = block();
			ret = new Program(tmp, blo);
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

Block* Mer::Parser::block()
{
	mem.new_block();
	this_namespace->sl_table->new_block();
	token_stream.match(BEGIN);
	Block* ret = new Block();
	while (token_stream.this_tag() != END)
	{
		ParserNode* node;
		switch (token_stream.this_tag())
		{
		case BEGIN:
			node = block();
			break;
		case SWITCH:
			node = switch_statement();
			break;
		case FOR:
			node = for_statement();
			break;
		case DO:
		case WHILE:
			node = while_statement();
			break;
		case IF:
			node = if_statement();
			break;
		default:
			node = statement();
			break;
		}
		ret->ins_table.push_back(node);
	}
	token_stream.match(END);
	mem.end_block();
	this_namespace->sl_table->end_block();
	return ret;
}
//served for function;
// !!!! Note that pure_block will run mem.end_block(), but not mem.new_block();
Block* Mer::Parser::pure_block()
{
	token_stream.match(BEGIN);
	Block* ret = new Block();
	current_function_block = ret;
	while (token_stream.this_tag() != END)
	{
		ParserNode* node;

		switch (token_stream.this_tag())
		{
		case BEGIN:
			node = block();
			break;
		case FOR:
			node = for_statement();
			break;
		case SWITCH:
			node = switch_statement();
			break;
		case DO:
		case WHILE:
			node = while_statement();
			break;
		case IF:
			node = if_statement();
			break;
		default:
			node = statement();
			break;
		}
		ret->ins_table.push_back(node);
	}
	token_stream.match(END);

	mem.end_block();
	return ret;
}

ParserNode* Mer::Parser::statement()
{
	ParserNode* node = nullptr;
	switch (token_stream.this_token()->get_tag())
	{
	case DELETE:
		node = new Delete();
		break;
	case ID:
		node = Parser::parse_id();
		break;
	case RETURN:
	{
		token_stream.match(RETURN);
		auto expr = new Expr();
		if (expr->get_type() != this_func_type) {
			throw Error("return type doesn't correspond with function type");
		}
		node = new Return(expr, current_function_block);
		break;
	}
	case BOOL_DECL:
	case INTEGER_DECL:
	case REAL_DECL:
	case STRING_DECL:
		node = var_decl();
		break;
	case BREAK:
		node = new Word(Word::Break);
		token_stream.match(BREAK);
		break;
	case CONTINUE:
		node = new Word(Word::Continue);
		token_stream.match(CONTINUE);
		break;
	case PRINT:
		token_stream.match(PRINT);
		node = new Print(token_stream.this_token());
		token_stream.next();
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
	size_t type = Mem::get_type_code(token_stream.this_token());
	std::vector<VarDeclUnit*> units;
	token_stream.next();
	units.push_back(new VarDeclUnit(type));
	//std::cout << token_stream.this_token()->to_string();
	while (token_stream.this_tag() != SEMI)
	{
		token_stream.match(COMMA);
		units.push_back(new VarDeclUnit(type));
	}
	if (global_stmt)
		return new GloVarDecl(units, type);
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


Mer::Print::Print(Token* tok) :content(tok)
{
	if (tok->get_tag() == ID)
	{
		auto result = this_namespace->sl_table->find(Id::get_value(tok));
		if (result == nullptr)
			throw Error(tok->to_string() + " no found the definition");
		if (result->es != ESymbol::SVAR)
			throw Error("print must pass a variable");
		pos = static_cast<VarIdRecorder*>(result)->pos;
	}
}

Mem::Object Mer::Print::execute()
{
	switch (content->get_tag())
	{
	case STRING:
		std::cout << String::get_value(content);
		break;
	case ID:
		std::cout << mem[mem.get_current() + pos]->to_string();
		break;
	default:
		throw Error("print: invalid args");
	}
	return nullptr;
}

Mem::Object Mer::Return::execute() {

	block->index = block->ins_table.size();

	block->ret = expr->execute();
	return nullptr;
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
		size = name_part.get_count();
		if (token_stream.this_tag() == ASSIGN)
		{
			token_stream.match(ASSIGN);
			expr = new InitList(type_code, name_part.get_count());
		}
		else {
			expr = new EmptyList(type_code, name_part.get_count());
		}
		return;
	}
	if (type_name_mapping.find(type_code) != type_name_mapping.end())
	{
		UStructure* result = find_ustructure_t(type_code);
		if (token_stream.this_tag() == BEGIN)
			expr = new StructureInitList(result->mapping);
		else {
			expr = new DefaultInitList(result->STMapping);
		}
		return;
	}
	if (token_stream.this_tag() == ASSIGN)
	{
		token_stream.match(ASSIGN);
		expr = (new Expr(type_code))->root();
		if (type_code != expr->get_type())
			throw Error("type not matched, from " + std::to_string(type_code) + " to " + std::to_string(expr->get_type()));
		return;
	}
	throw Error("try to init a non-init variable");
}

Mer::LocalVarDecl::LocalVarDecl(std::vector<VarDeclUnit*>& vec, size_t t) :type(t)
{
	for (const auto& a : vec)
	{
		sum += a->get_size();
	}
	pos = mem.push(sum) - sum;
	size_t tmp_pos = pos;
	if (vec[0]->pointer())
		this_namespace->sl_table->push(Id::get_value(vec[0]->get_id()), new VarIdRecorder(type+1, pos, ESymbol::SPOINTER));
	else
		this_namespace->sl_table->push(Id::get_value(vec[0]->get_id()), new VarIdRecorder(type, pos));
	for (int i = 1; i < vec.size(); i++)
	{
		if (vec[i]->pointer())
			this_namespace->sl_table->push(Id::get_value(vec[i]->get_id()), new VarIdRecorder(type+1, tmp_pos += vec[i - 1]->get_size(), ESymbol::SPOINTER));
		else
			this_namespace->sl_table->push(Id::get_value(vec[i]->get_id()), new VarIdRecorder(type, tmp_pos += vec[i - 1]->get_size()));
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
	this_namespace->sl_table->push(Id::get_value(vec[0]->get_id()), new GVarIdRecorder(type, pos));
	for (int i = 1; i < vec.size(); i++)
	{
		this_namespace->sl_table->push(Id::get_value(vec[i]->get_id()), new GVarIdRecorder(type, tmp_pos += vec[i - 1]->get_size()));
	}
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
	for (int i = 0; i < sum; i++) {
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
