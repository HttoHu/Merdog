/*
* Inspired by
* https://ruslanspivak.com/lsbasi-part10/
* Ruslan's Blog
* C++ Version.
* Yuantao Hu 2018
*/
#include "../include/memory.hpp"
#include "../include/branch_statement.hpp"
#include "../include/loop_statement.hpp"
#include "../include/function.hpp"
#include "../include/environment.hpp"
using namespace Mer;

AST * Mer::Parser::parse()
{
	auto node = program();
	return node;
}

Program * Mer::Parser::program()
{
	Program *ret=nullptr;
	int programe_num = 0;
	while (1)
	{
		switch (token_stream.this_tag())
		{
		case FUNCTION:
			build_function();
			break;
		case PROGRAM:
		{
			symbol_table.push();
			token_stream.match(PROGRAM);
			auto tmp = token_stream.this_token();
			token_stream.match(ID);
			std::string name = Id::get_value(tmp);
			_mem.new_block();
			auto blo = block();
			_mem.end_block();
			token_stream.match(DOT);
			ret = new Program(Id::get_value(tmp), blo);
			programe_num++;
			symbol_table.pop();
			break;
		}
		case ENDOF:
			if (programe_num != 1)
				throw Error("must have a program as an entry");
			return ret;
		default:
			break;
		}
	}

}

Block * Mer::Parser::block()
{
	id_pos_table.push_front(std::map<Mer::Token*, std::size_t>());
	Block *ret = new Block();
	ret->compound_list = static_cast<Compound*>(compound_statement());
	id_pos_table.pop_front();
	return ret;
}

VarDecl * Mer::Parser::variable_declaration()
{
	std::map<Token*, Expr*> var_list;
	auto type = type_spec();
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
	VarDecl *ret = new VarDecl();
	ret->type = static_cast<Type*>(type);
	ret->init_var_list(var_list);
	return ret;
}

Type * Mer::Parser::type_spec()
{
	auto tmp = token_stream.this_token();
	switch (tmp->get_tag())
	{
	case REF:token_stream.match(REF); break;
	case INTEGER_DECL:token_stream.match(INTEGER_DECL); break;
	case REAL_DECL:token_stream.match(REAL_DECL); break;
	case STRING_DECL:token_stream.match(STRING_DECL); break;
	case BOOL_DECL:token_stream.match(BOOL_DECL); break;
	default:
		throw Error("no matched type " + tmp->to_string());
	}
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

CallFunc * Mer::Parser::func_call()
{
	Token *func_name = token_stream.this_token();
	std::vector<Expr*> exprs;
	token_stream.match(ID);
	token_stream.match(LPAREN);
	if(token_stream.this_tag()==RPAREN)
	{
		token_stream.match(RPAREN);
		return new CallFunc(func_name, exprs);
	}
	exprs.push_back(new Expr());
	while (token_stream.this_tag() == COMMA)
	{
		token_stream.match(COMMA);
		exprs.push_back(new Expr());
	}
	token_stream.match(RPAREN);
	return new CallFunc(func_name, exprs);
}

AST * Mer::Parser::statement()
{
	AST *node = nullptr;
	switch (token_stream.this_token()->get_tag())
	{

	case FOR:
		node = for_statement();
		break;
	case RETURN:
		token_stream.match(RETURN);
		if (token_stream.this_tag() == SEMI)
			return new Return(nullptr);
		else
			return new Return(new Expr());
	case BREAK:
		node = new Word(Word::Type::Break);
		token_stream.match(BREAK);
		break;
	case CONTINUE:
		node = new Word(Word::Type::Continue);
		token_stream.match(CONTINUE);
		break;
	case WHILE:
		node = while_statement();
		break;
	case IF:
		node = if_statement();
		break;
	case BEGIN:
		node = compound_statement();
		break;
	case ID:
		node = id_event();
		break;
	case REF:
	case BOOL_DECL:
	case INTEGER_DECL:
	case REAL_DECL:
	case STRING_DECL:
		node = variable_declaration();
		break;
	case PRINT:
		node = print_statement();
		break;
	default:
		node = empty();
		break;
	}
	return node;
}

AST * Mer::Parser::id_event()
{
	if (token_stream.next_token()->get_tag() == LPAREN)
	{
		return func_call();
	}
	else
		return assignment_statement();
}

AST * Mer::Parser::print_statement()
{
	token_stream.match(PRINT);
	auto node = new Print(token_stream.this_token());
	token_stream.next();
	return node;
}

AST * Mer::Parser::assignment_statement()
{
	auto left = variable();
	auto token = token_stream.this_token();
	token_stream.next();
	token_stream.this_token()->to_string();
	auto expr = new Expr();
	auto node = new AST();
	//{a:=4}.
	if (expr->get_type() != left->get_type())
	{
		throw Error("type not matched");
	}
	switch (token->get_tag())
	{
	case ASSIGN:
		node = new Assign(static_cast<Var*>(left)->get_pos(), token, expr->root());
		break;
	case SPLUS:
		node = new AssignPlus(static_cast<Var*>(left)->get_pos(), token, expr->root());
		break;
	case SMINUS:
		node = new AssignMinus(static_cast<Var*>(left)->get_pos(), token, expr->root());
		break;
	case SMUL:
		node = new AssignMul(static_cast<Var*>(left)->get_pos(), token, expr->root());
		break;
	case SDIV:
		node = new AssignDiv(static_cast<Var*>(left)->get_pos(), token, expr->root());
		break;
	default:
		break;
	}

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

Mem::Object Mer::Assign::get_value()
{
	return _mem[left]->operator=(right->get_value());
}

void Mer::Block::new_block()
{
	_mem.new_block();
}

void Mer::Block::end_block()
{
	_mem.end_block();
}

Mem::Object Mer::Block::get_value()
{
	return compound_list->get_value();
}

Mem::Object Mer::VarDecl::get_value()
{
	for (const auto &a : var_list)
	{
		if (a.second != nullptr)
		{
			_mem[a.first] = type->adapt_value(a.second->get_value());
			//invalid initialization of non-const reference of type 
			//'Mer::Mem::Object& {aka std::shared_ptr<Mer::Mem::Value>&}' from an rvalue of type 'Mer::Mem::Object {aka std::shared_ptr<Mer::Mem::Value>}'
		}
		else
		{
			_mem[a.first] = type->emit_var();
		}
	}
	return nullptr;
}

void Mer::VarDecl::init_var_list(const std::map<Token*, Expr*>& v)
{
	for (const auto &a : v)
	{
		if (type->get_type() == Mem::BasicType::TREF)
		{ }
		else if (a.second->get_type() != type->get_type()&&type->get_type()!=Mem::BasicType::TREF)
			throw Error(Mem::type_to_string(Mem::BasicType(type->get_type())) 
				+" type not matched with "+ Mem::type_to_string(Mem::BasicType(a.second->get_type()))
			);
		auto pos = _mem.push();
		id_pos_table.front().insert({ a.first,pos });
		var_list.insert({ pos, a.second });
		symbol_table.insert_basic(a.first, IdType::TVar);
		symbol_table.insert_type(a.first, a.second->get_type());
	}
}

Mer::Var::Var(Token * t)
{
	pos = find_pos(t);
	type = symbol_table.find_type(t);
}

Mem::Object Mer::Var::get_value()
{
	return _mem[pos];
}

Mem::Object Mer::Print::get_value()
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

Mem::Object Mer::AssignPlus::get_value()
{
	return _mem[left]->operator+=(right->get_value());
}

Mem::Object Mer::AssignMinus::get_value()
{
	return _mem[left]->operator-=(right->get_value());
}

Mem::Object Mer::AssignMul::get_value()
{
	return _mem[left]->operator*=(right->get_value());
}

Mem::Object Mer::AssignDiv::get_value()
{
	return _mem[left]->operator/=(right->get_value());
}

Mer::CallFunc::CallFunc(Token * fun, std::vector<Expr*>& vec):args(vec)
{
	rvs = _mem.get_index();
	auto result = function_map.find(Id::get_value(fun));
	if (result == function_map.end())
		throw Error("function "+fun->to_string() + " no found");
	func = result->second;
}

Mem::Object Mer::CallFunc::get_value()
{
	std::vector<Mem::Object> tmp;
	for (const auto &a : args)
	{
		tmp.push_back(a->get_value());
	}
	return function_list[func]->call(tmp, rvs);
}

size_t Mer::CallFunc::get_type()
{
	return function_list[func]->get_type();
}
