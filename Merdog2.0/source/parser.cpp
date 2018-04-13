#include "../include/parser.hpp"
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
#include "../include/if.hpp"
#include "../include/loop_statement.hpp"
using namespace Mer;

Program* Mer::Parser::program()
{
	token_stream.match(PROGRAM);
	auto tmp = token_stream.this_token();
	token_stream.match(ID);
	std::string name = Id::get_value(tmp);
	auto blo = block();
	token_stream.match(DOT);
	return new Program(tmp, blo);
}

Block * Mer::Parser::block()
{
	stack_memory.new_block();
	global_symbol_table.new_block();
	token_stream.match(BEGIN);
	Block *ret = new Block();
	while (token_stream.this_tag() != END)
	{
		ParserNode *node;
		switch (token_stream.this_tag())
		{
		case BEGIN:
			node = block();
			break;
		case FOR:
			node = for_statement();
			break;
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
	stack_memory.end_block();
	global_symbol_table.end_block();
	return ret;
}

Block * Mer::Parser::pure_block()
{
	token_stream.match(BEGIN);
	Block *ret = new Block();
	while (token_stream.this_tag() != END)
	{
		ParserNode *node;

		switch (token_stream.this_tag())
		{
		case BEGIN:
			node = block();
			break;
		case FOR:
			node = for_statement();
			break;
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
	stack_memory.end_block();
	return ret;
}

ParserNode * Mer::Parser::statement()
{
	ParserNode *node = nullptr;
	switch (token_stream.this_token()->get_tag())
	{
	case ID:
		node = assignment_statement();
		break;
	case BOOL_DECL:
	case INTEGER_DECL:
	case REAL_DECL:
	case STRING_DECL:
		node = var_decl();
		break;
	case BREAK:
		node = new Word(Word::Type::Break);
		token_stream.match(BREAK);
		break;
	case CONTINUE:
		node = new Word(Word::Type::Continue);
		token_stream.match(CONTINUE);
		break;
	case PRINT:
		token_stream.match(PRINT);
		node = new Print(token_stream.this_token());
		token_stream.next();
		break;
	default:
		throw Error("Unknown syntax please update the Merdog or find whether you have made a mistake. ");
	}
	token_stream.match(SEMI);
	return node;
}

Assign * Mer::Parser::assignment_statement()
{
	auto name = token_stream.this_token();
	auto left = get_current_info();

	if (left->es != ESymbol::SVAR)
		throw Error(name->to_string() + " is not a lv");
	size_t pos = static_cast<VarIdRecorder*>(left)->pos;
	token_stream.match(ID);
	Assign::AssignType assignment_type = Assign::AssignType::None;
	switch (token_stream.this_tag())
	{
	case SADD:
		assignment_type = Assign::AssignType::Add;
		break;
	case SSUB:
		assignment_type = Assign::AssignType::Sub;
		break;
	case SMUL:
		assignment_type = Assign::AssignType::Mul;
		break;
	case SDIV:
		assignment_type = Assign::AssignType::Div;
		break;
	default:
		break;
	}
	token_stream.next();
	auto expr = new Expr();
	auto node = new Assign(assignment_type, pos, name, expr->root());
	delete expr;
	return node;
}

VarDecl * Mer::Parser::var_decl()
{
	std::map<Token*, Expr*> var_list;
	// get types
	auto type = token_stream.this_token();
	size_t type_code = Mem::get_type_code(type);
	token_stream.next();
	//matched the exprs with the ids and push into var_list to init a VarDecl.
	auto id = token_stream.this_token();

	token_stream.match(ID);
	if (token_stream.this_token()->get_tag() == ASSIGN)
	{
		token_stream.match(ASSIGN);
		auto exp = new Expr();
		var_list.insert({ id,exp });
	}
	else
		throw Error("try to create a var without initialization");
	while (token_stream.this_token()->get_tag() == COMMA)
	{
		auto id = token_stream.this_token();
		token_stream.match(ID);
		if (token_stream.this_token()->get_tag() == ASSIGN)
		{
			token_stream.match(ASSIGN);
			auto exp = new Expr();
			var_list.insert({ id,exp });
		}
		else
		{
			var_list.insert({ id,nullptr });
		}
	}
	return new VarDecl(type_code, var_list);
}

WordRecorder * Mer::Parser::get_current_info()
{
	return global_symbol_table.find(Mer::Id::get_value(token_stream.this_token()));
}

Mer::VarDecl::VarDecl(size_t t, const std::map<Token*, Expr*>& v)
{
	type = t;
	for (const auto &a : v)
	{
		if (global_symbol_table.find_front(Id::get_value(a.first)) != nullptr)
			throw Error("Symbol " + a.first->to_string() + " redefined");
		auto pos = stack_memory.push();
		global_symbol_table.push(Id::get_value(a.first), new VarIdRecorder(t, pos));
		var_list.push_back({ pos, a.second });
	}
}

Mem::Object Mer::VarDecl::execute()
{
	for (const auto &a : var_list)
	{
		if (a.second != nullptr)
			stack_memory[a.first] = a.second->execute();
		else
			throw Error("please init var");
	}
	return nullptr;
}

Mer::Print::Print(Token * tok) :content(tok)
{
	if (tok->get_tag() == ID)
	{
		auto result = global_symbol_table.find(Id::get_value(tok));
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
		std::cout << stack_memory[pos]->to_string();
		break;
	default:
		throw Error("print: invalid args");
	}
	return nullptr;
}