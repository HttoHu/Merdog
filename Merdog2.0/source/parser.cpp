#include "../include/parser.hpp"
#include "../include/memory.hpp"
using namespace Mer;
std::unordered_map<std::string, Mer::Mem::Raw> Mer::glo_mem;

void Mer::Parser::print_var_list()
{
	std::cout << "{";
	for (const auto &a : glo_mem)
	{
		std::cout << "\'" + a.first + "\'" + ":" << a.second->to_string() << ", ";
	}
	std::cout << "}";
}

AST * Mer::Parser::parse()
{
	auto node = program();
	return node;
}

Program * Mer::Parser::program()
{
	token_stream.match(PROGRAM);
	auto tmp = token_stream.this_token();
	token_stream.match(ID);
	std::string name = Id::get_value(tmp);
	token_stream.match(SEMI);
	auto blo = block();
	token_stream.match(DOT);
	return new Program(name, blo);

	/*
	auto node = compound_statement();
	token_stream.match(DOT);
	return node;
	*/
}

Block * Mer::Parser::block()
{
	Block *ret = new Block();
	ret->compound_list = static_cast<Compound*>(compound_statement());
	
	return ret;
}

VarDecl * Mer::Parser::variable_declaration()
{
	std::map<Token*,Expr*> var_list;
	auto type = type_spec();
	auto id = token_stream.this_token();
	token_stream.match(ID);
	if (token_stream.this_token()->get_tag()== ASSIGN)
	{
		token_stream.match(ASSIGN);
		auto exp= new Expr();
		var_list.insert({ id,exp });
	}
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
	}
	VarDecl *ret = new VarDecl();
	ret->type = static_cast<Type*>(type);
	ret->init_var_list(var_list);
	return ret;
}

AST * Mer::Parser::type_spec()
{
	auto tmp = token_stream.this_token();
	if (tmp->get_tag() == INTEGER_DECL)
		token_stream.match(INTEGER_DECL);
	else
		token_stream.match(REAL_DECL);
	return new Type(tmp);
}

AST * Mer::Parser::compound_statement()
{
	token_stream.match(BEGIN);
	auto nodes = statemnet_list();
	token_stream.match(END);
	auto root = new Compound();
	for (const auto &a : nodes)
		root->children.push_back(a);
	return root;
}

std::vector<AST*> Mer::Parser::statemnet_list()
{
	auto node = statement();
	std::vector<AST*> results{ node };
	while (token_stream.this_token()->get_tag() == SEMI)
	{
		token_stream.match(SEMI);
		results.push_back(statement());
	}
	if (token_stream.this_token()->get_tag() == ID)
		throw Error("syntax error");
	return results;
}

AST * Mer::Parser::statement()
{
	AST *node = nullptr;
	if (token_stream.this_token()->get_tag() == BEGIN)
		node = compound_statement();
	else if (token_stream.this_token()->get_tag() == ID)
		node = assignment_statement();
	else if (token_stream.this_token()->get_tag() == INTEGER_DECL || token_stream.this_token()->get_tag() == REAL_DECL)
		node = variable_declaration();
	else if (token_stream.this_token()->get_tag() == PRINT)
		node=print_statement();
	else
		node = empty();
	return node;
}

AST * Mer::Parser::print_statement()
{
	token_stream.match(PRINT);
	auto node = new Print(token_stream.this_token());
	token_stream.advance();
	return node;
}

AST * Mer::Parser::assignment_statement()
{
	auto left = variable();
	auto token = token_stream.this_token();
	token_stream.match(ASSIGN);
	token_stream.this_token()->to_string();
	auto expr = new Expr();

	//{a:=4}.
	auto node = new Assign(static_cast<Var*>(left)->get_pos(), token, expr->root());
	delete expr;
	return node;
}
AST * Mer::Parser::variable()
{
	auto node = new Var(token_stream.this_token());
	token_stream.match(ID);
	return node;
}
AST * Mer::Parser::empty()
{
	return new NoOp;
}

Mem::Raw Mer::Assign::get_value()
{
	auto ret = _mem[left]->operator=(&*right->get_value());
	return std::shared_ptr<Mem::Value>(ret);
}

Mem::Raw Mer::Block::get_value()
{
	return compound_list->get_value();
}

Mem::Raw Mer::VarDecl::get_value()
{
	for (const auto &a : var_list)
	{
		if (a.second != nullptr)
		{
			_mem[a.first] = a.second->get_value();
		}
		else
			_mem[a.first] = type->emit_var();
	}
	return nullptr;
}

void Mer::VarDecl::init_var_list(const std::map<Token*,Expr*>& v)
{
	for (const auto &a : v)
	{
		auto pos = _mem.push();
		id_pos_table.back().insert({ a.first,pos });
		var_list.insert({ pos, a.second });
	}
}

Mer::Var::Var(Token * t)
{
	pos = find_pos(t);
}

Mem::Raw Mer::Var::get_value()
{
	return _mem[pos];
}

Mem::Raw Mer::Print::get_value()
{
	if (tok->get_tag() == STRING)
		std::cout << String::get_value(tok);
	else if (tok->get_tag() == ID)
	{
		auto tmp = find_pos(tok);
		std::cout << _mem[tmp]->to_string();
	}
	return nullptr;
}
