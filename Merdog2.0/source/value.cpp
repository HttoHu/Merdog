#include "../include/value.hpp"
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
#include "../include/parser.hpp"
#include "../include/function.hpp"
#include "../include/environment.hpp"
Mer::Variable::Variable(Token * tok)
{
	auto result = global_symbol_table.find(Id::get_value(tok));
	if (result == nullptr)
		throw Error("var " + tok->to_string() + " no found");
	if (result->es != ESymbol::SVAR)
		throw Error(tok->to_string() + " must be a symbol");
	pos = static_cast<VarIdRecorder*> (result)->pos;
}

Mer::Mem::Object Mer::Variable::execute()
{
	return stack_memory[pos];
}

Mer::FunctionCall::FunctionCall(Token * func_name, std::vector<Expr*>& exprs)
{
	func = function_table.find(Id::get_value(func_name))->second;
	argument = exprs;
}

Mer::Mem::Object Mer::FunctionCall::execute()
{
	return func->run(argument);
}
Mer::ParserNode * Mer::Parser::parse_id()
{
	ParserNode *ret = nullptr;
	auto id = token_stream.this_token();
	auto result = global_symbol_table.find(Id::get_value(id));
	if (result == nullptr)
		throw Error("no found the definition of the id " + id->to_string());
	switch (result->es)
	{
	case ESymbol::SFUN:
		 ret=parse_function_call();
	case ESymbol::SVAR:
	{
		ret= new Variable(token_stream.this_token());
		token_stream.match(ID);
		break;
	}
	default:
		throw Error("please updata your Merdog interpreter or check whether have the errors in your program.");
	}
	Assign::AssignType assignment_type;
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
		return ret;
	}
	size_t pos = static_cast<VarIdRecorder*>(result)->pos;
	token_stream.next();
	auto expr = new Expr();
	auto node = new Assign(assignment_type, pos, id, expr->root());
	delete expr;
	return node;
}
Mer::FunctionCall * Mer::Parser::parse_function_call()
{
	auto id = token_stream.this_token();
	std::vector<Expr*> exprs;
	global_symbol_table.type_check(id, Mer::ESymbol::SFUN);
	auto result = function_table.find(Id::get_value(id));
	token_stream.match(ID);
	if (result == function_table.end())
		throw Error("function " + id->to_string() + " no found its defination");
	auto function = result->second;
	token_stream.match(LPAREN);
	if (token_stream.this_tag() == RPAREN)
	{
		token_stream.match(RPAREN);
		return new FunctionCall(id, exprs);
	}
	exprs.push_back(new Expr());
	while (token_stream.this_tag() == COMMA)
	{
		token_stream.match(COMMA);
		exprs.push_back(new Expr());
	}
	token_stream.match(RPAREN);
	return new FunctionCall(id, exprs);
}
