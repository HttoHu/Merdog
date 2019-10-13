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

Mer::FunctionCall::FunctionCall(const std::vector<size_t>& types, size_t _index, FunctionBase* _func, std::vector<Expr*>& exprs) :index(_index), func(_func), argument(exprs)
{
	func->check_param(types);
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
	func->set_index(index);
	return func->run(tmp);
}

std::string Mer::FunctionCall::to_string()
{
	std::string str="function_pos:";
	str += std::to_string(func->index);
	str += "(";
	for (auto &a : argument)
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
	case ESymbol::SGVAR:
	{
		return parse_glo(result);
	}
	case ESymbol::SSTRUCTURE:
	{
		return var_decl();
	}
	case ESymbol::STYPE:// Build-in Type but not basic type 
	{
		return static_cast<BuildInClass*>(result)->create_var();
	}
	case ESymbol::SFUN:
		return parse_function_call(this_namespace);
	case ESymbol::SARRAY:
	case ESymbol::SVAR:
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
	switch (token_stream.this_tag())
	{
	case LSB:
	{
		token_stream.back();
		return parse_array(var_info);
	}
	default:
		return new Variable(var_info);
	}
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

Mer::FunctionCall* Mer::Parser::parse_function_call(Namespace* names)
{
	auto id = token_stream.this_token();
	std::vector<Expr*> exprs;
	// to check the param's type.
	std::vector<size_t> param_types;
	names->sl_table->type_check(id, Mer::ESymbol::SFUN);
	auto result = names->find_func(Id::get_value(id));
	token_stream.match(ID);
	if (result == nullptr)
		throw Error("function " + id->to_string() + " no found its defination");
	token_stream.match(LPAREN);
	if (token_stream.this_tag() == RPAREN)
	{
		token_stream.match(RPAREN);
		return new FunctionCall(param_types, mem.get_index(), result, exprs);
	}
	auto param_unit = new Expr();
	param_types.push_back(param_unit->get_type());
	exprs.push_back(param_unit);
	while (token_stream.this_tag() == COMMA)
	{
		token_stream.match(COMMA);
		auto param_unit2 = new Expr();
		param_types.push_back(param_unit2->get_type());
		exprs.push_back(param_unit2);
	}
	token_stream.match(RPAREN);
	return new FunctionCall(param_types, mem.get_index(), result, exprs);
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


Mer::ParserNode* Mer::Parser::parse_array(WordRecorder* var_info)
{
	auto id_name = token_stream.this_token();
	token_stream.match(ID);
	token_stream.match(LSB);
	size_t type = static_cast<VarIdRecorder*>(var_info)->get_type();
	// if the value bewteen [] is a constant.
	if (token_stream.this_tag() == INTEGER)
	{
		int off = Integer::get_value(token_stream.this_token());
		token_stream.match(INTEGER);
		token_stream.match(RSB);
		// if the var is not an array but supports [] operator.
		if (var_info->es != SARRAY)
		{
			return new Index(new Variable(type, var_info->get_pos()), off);
		}
		return new Variable(type, var_info->get_pos() + off);
	}
	auto expr = new Expr();
	token_stream.match(RSB);
	if (var_info->get_type() >= USER_TYPE_INDEX)
	{
		throw Error("haven't finished yet");
	}
	// if the var is not an array but supports [] operator. and the expr of the []is not a constant.
	if (var_info->es != SARRAY)
	{
		return new BinOp(new Variable(type, var_info->get_pos()), new Token(LSB), expr);
	}
	return new ContainerIndex(type, var_info->get_pos(), expr);
}

// the same function of parse_member but used to parse glo var
Mer::ParserNode* Mer::Parser::parse_glo(WordRecorder* var_info)
{
	auto id_name = token_stream.this_token();
	token_stream.match(ID);
	if (token_stream.this_tag() != LSB)
	{
		return new GVar(var_info);
	}
	token_stream.match(LSB);
	if (token_stream.this_tag() == INTEGER)
	{
		int off = Integer::get_value(token_stream.this_token());
		token_stream.match(INTEGER);
		token_stream.match(RSB);
		return new GVar(var_info, off);
	}
	auto expr = new Expr();
	token_stream.match(RSB);
	return new ContainerIndex(static_cast<VarIdRecorder*>(var_info)->pos, static_cast<VarIdRecorder*>(var_info)->get_type(), expr);
}