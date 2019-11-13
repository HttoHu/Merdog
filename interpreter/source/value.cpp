/*
*	MIT License
*	Copyright (c) 2019 Htto Hu
*/
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
#include "../include/environment.hpp"
#include "../include/compound_box.hpp"

Mer::Variable::Variable(WordRecorder* wr)
{
	pos = static_cast<VarIdRecorder*>(wr)->pos;
	type = wr->get_type();
}

size_t Mer::Variable::get_type()
{
	return type;
}

size_t Mer::Variable::get_pos() {
	return pos;
}

Mer::Mem::Object Mer::Variable::execute()
{
	return mem[mem.get_current() + pos];
}

Mer::FunctionCall::FunctionCall( FunctionBase* _func, std::vector<ParserNode*>& exprs) : func(_func), argument(exprs)
{
	std::vector<size_t> type_vec;
	for (auto& a : exprs)
	{
		type_vec.push_back(a->get_type());
	}
	func->check_param(type_vec);
	std::vector<ParserNode*> tmp;
	func->convert_arg(argument);
}

size_t Mer::FunctionCall::get_type()
{
	return func->get_type();
}

Mer::Mem::Object Mer::FunctionCall::execute()
{
	std::vector<Mem::Object> tmp;
	for (const auto& a : argument)
	{
		tmp.push_back(a->execute()->clone());
	}
	return func->run(tmp);
}

std::string Mer::FunctionCall::to_string()
{
	std::string str = "function_pos:";
	str += std::to_string(func->function_offset);
	str += "(";
	for (auto& a : argument)
		str += a->to_string();
	str += ")";
	return str;
}


Mer::ParserNode* Mer::Parser::parse_id()
{

	auto id = token_stream.this_token();
	auto result = this_namespace->sl_table->find(Id::get_value(id));
	if (result == nullptr)
	{
		auto target_namespace = kill_namespaces();
		if (target_namespace == nullptr)
			throw Error("Merdog is broken");
		return _parse_id_wn(target_namespace);
	}
	switch (result->es)
	{
	case ESymbol::SGARR:
	case ESymbol::SGVAR:
		return parse_glo(result);
	case ESymbol::SSTRUCTURE:
		return var_decl();
	case ESymbol::SFUN:
		return parse_function_call(this_namespace);
	case ESymbol::SARRAY:
	case ESymbol::SVAR:
	case ESymbol::MVAR:
		return parse_var(result);
	default:
	{
		auto target_namespace = kill_namespaces();
		if (target_namespace == nullptr)
			throw Error("unsupported id type");
		return _parse_id_wn(target_namespace);
	}
	}
}

Mer::ParserNode* Mer::Parser::parse_var(WordRecorder* var_info)
{
	Token* var_id = token_stream.this_token();
	token_stream.match(ID);
	// if var is struct member
	if (var_info->es == ESymbol::MVAR)
	{
		return new Index(new Variable((size_t)(Mem::type_counter) + 1, 0), var_info->get_pos(), var_info->get_type());
	}
	auto ret = new Variable(var_info);
	if (var_info->es == SARRAY)
		ret->arr() = true;
	return ret;
}



Mer::ParserNode* Mer::Parser::_parse_id_wn(Namespace* names)
{
	auto id = token_stream.this_token();
	auto result = names->sl_table->find(Id::get_value(id));
	if (result == nullptr)
		throw Error(id->to_string() + " no found");
	switch (result->es)
	{
	case ESymbol::SFUN:

		return parse_function_call(names);
	case ESymbol::SGVAR:
	{
		return parse_glo(result);
	}
	default:
		throw Error("Unsupported id type");
	}
}

Mer::FunctionCall* Mer::Parser::parse_initializer(size_t type)
{
	auto result = type_init_function_map.find(type);
	if (result == type_init_function_map.end())
		throw Error("type " + type_to_string(type) + " don't support initializer");
	auto func = result->second;
	std::vector<ParserNode*> exprs = parse_arguments();
	return new FunctionCall(func, exprs);
}

std::vector<Mer::ParserNode*> Mer::Parser::parse_arguments()
{
	std::vector<ParserNode*> exprs;
	token_stream.match(LPAREN);
	if (token_stream.this_tag() == RPAREN)
	{
		token_stream.match(RPAREN);
		return exprs;
	}
	auto param_unit = Expr().root();
	exprs.push_back(param_unit);
	while (token_stream.this_tag() == COMMA)
	{
		token_stream.match(COMMA);
		auto param_unit2 = Expr().root();
		exprs.push_back(param_unit2);
	}
	token_stream.match(RPAREN);
	return exprs;
}

Mer::FunctionCall* Mer::Parser::parse_function_call(Namespace* names)
{
	auto id = token_stream.this_token();
	// to check the param's type.
	names->sl_table->type_check(id, Mer::ESymbol::SFUN);
	auto result = names->find_func(Id::get_value(id));
	token_stream.match(ID);
	if (result == nullptr)
		throw Error("function " + id->to_string() + " no found its defination");
	std::vector<ParserNode*> exprs = parse_arguments();
	return new FunctionCall(result, exprs);
}

Mer::FunctionCall* Mer::Parser::parse_call_by_function(FunctionBase* f, ParserNode* parent)
{
	std::vector<ParserNode*> exprs;
	// to check the param's type.
	//get ref of parent
	exprs.push_back(parent);
	token_stream.match(LPAREN);
	if (token_stream.this_tag() == RPAREN)
	{
		token_stream.match(RPAREN);
		return new FunctionCall( f, exprs);
	}
	auto param_unit = new Expr();
	exprs.push_back(param_unit);
	while (token_stream.this_tag() == COMMA)
	{
		token_stream.match(COMMA);
		auto param_unit2 = new Expr();
		exprs.push_back(param_unit2);
	}
	token_stream.match(RPAREN);
	return new FunctionCall( f, exprs);
}

Mer::Namespace* Mer::Parser::kill_namespaces()
{
	Namespace* current = this_namespace;
	auto result = _find_namespace_driver(current, Id::get_value(token_stream.this_token()));
	while (result != nullptr)
	{
		current = result;
		token_stream.match(ID);
		token_stream.match(DOT);
		result = _find_namespace_driver(current, Id::get_value(token_stream.this_token()));
	}
	return current;
}

Mer::LConV::LConV(Token* t)
{
	switch (t->get_tag())
	{
	case TTRUE:
		type = Mem::BOOL;
		obj = std::make_shared<Mem::Bool>(true);
		break;
	case TFALSE:
		type = Mem::BOOL;
		obj = std::make_shared<Mem::Bool>(false);
		break;
	case INTEGER:
		type = Mem::INT;
		obj = std::make_shared<Mem::Int>(Integer::get_value(t));
		break;
	case REAL:
		type = Mem::DOUBLE;
		obj = std::make_shared <Mem::Double >(Real::get_value(t));
		break;
	case STRING:
		type = Mem::STRING;
		obj = std::make_shared<Mem::String>(String::get_value(t));
		break;
	case CHAR_LIT:
		type = Mem::CHAR;
		obj = std::make_shared<Mem::Char>(CharToken::get_value(t));
		break;
	default:
		throw Error("syntax error");
	}
}

Mer::GVar::GVar(WordRecorder* result)
{
	pos = static_cast<GVarIdRecorder*>(result)->pos;
	type = result->get_type();
}

Mer::GVar::GVar(WordRecorder* result, size_t offset)
{
	pos = static_cast<GVarIdRecorder*>(result)->pos + offset;
	type = result->get_type();
}

// the same function of parse_member but used to parse glo var
Mer::ParserNode* Mer::Parser::parse_glo(WordRecorder* var_info)
{
	// Similar to parse_array;
	auto id_name = token_stream.this_token();
	token_stream.match(ID);
	return new GVar(var_info);
}
