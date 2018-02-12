#include "../include/parser.hpp"
using namespace Mer;
std::unordered_map<std::string, Value> Mer::glo_mem;

void Mer::Parser::print_var_list()
{
	std::cout << "{";
	for (const auto &a : glo_mem)
	{
		std::cout << "\'" + a.first + "\'" + ":" << a.second<<", ";
	}
	std::cout << "}";
}

AST * Mer::Parser::parse()
{
	auto node = program();
	return node;
}

AST * Mer::Parser::program()
{
	auto node = compound_statement();
	token_stream.match(DOT);
	return node;
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
	else
		node = empty();
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
	auto node = new Assign(left, token, expr->root());
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

Value Mer::Assign::get_value()
{

	std::string var_name = Var::get_name(static_cast<Var*>(left));
	glo_mem[var_name] = right->get_value();
	return 0;
}
