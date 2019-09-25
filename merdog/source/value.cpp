/*
* MIT License
* Copyright (c) 2019 Htto Hu
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

Mer::Mem::Object Mer::Variable::execute()
{
	return mem[mem.get_current()+pos];
}

Mer::FunctionCall::FunctionCall(const std::vector<size_t> &types, size_t _index, FunctionBase * _func, std::vector<Expr*>& exprs) :index(_index), func(_func), argument(exprs)
{
	if (!func->check_param(types))
	{
		throw Error("A02 type not matched ");
	}
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
	for (const auto &a : argument)
	{
		tmp.push_back(a->execute());
	}
	func->set_index(index);
	return func->run(tmp);
}
// How to split the method into some smaller functions?? 6-7-18
Mer::Assign::AssignType _token_to_assType()
{
	using namespace Mer;
	switch (token_stream.this_tag())
	{
	case ASSIGN:
		return Assign::AssignType::None;
	case SADD:
		return Assign::AssignType::Add;
	case SSUB:
		return Assign::AssignType::Sub;
	case SMUL:
		return Assign::AssignType::Mul;
	case SDIV:
		return Assign::AssignType::Div;
	default:
		return Assign::AssignType::Null;
	}
}

Mer::ParserNode* Mer::Parser::parse_member(WordRecorder* var_info,size_t offset=0)
{
	size_t pos = static_cast<VarIdRecorder*>(var_info)->pos + offset;
	token_stream.match(DOT);
	std::string member_name = Id::get_value(token_stream.this_token());
	auto usinfo = find_ustructure_t(var_info->get_type());
	auto member_info = usinfo->STMapping.find(member_name);
	if (member_info == usinfo->STMapping.end())
		throw Error("member " + member_name + " no found");
	token_stream.match(ID);
	return new Index(new Variable(member_info->second,pos),member_info->second);
}

Mer::ParserNode* Mer::Parser::parse_member_glo(WordRecorder* var_info, size_t offset)
{
	size_t pos = static_cast<VarIdRecorder*>(var_info)->pos + offset;
	token_stream.match(DOT);
	std::string member_name = Id::get_value(token_stream.this_token());
	auto usinfo = find_ustructure_t(var_info->get_type());
	auto member_info = usinfo->mapping.find(member_name);
	if (member_info == usinfo->mapping.end())
		throw Error("member " + member_name + " no found");
	token_stream.match(ID);
	return new Index(new GVar(var_info,offset), member_info->second);
}

Mer::ParserNode* Mer::Parser::parse_id()
{
	ParserNode *ret = nullptr;
	auto id = token_stream.this_token();
	auto result = this_namespace->sl_table->find(Id::get_value(id));
	if (result == nullptr)
	{
		auto target_namespace = kill_namespaces();
		if (target_namespace == nullptr)
			throw Error("Merdog is broken");
		ret = _parse_id_wn(target_namespace);
		return ret;
	}
	switch (result->es)
	{
	case ESymbol::SGVAR:
	{
		ret = parse_glo(result);
		break;
	}
	case ESymbol::SSTRUCTURE:
	{
		return var_decl();;
	}
	case ESymbol::STYPE:// Build-in Type but not basic type 
	{
		return static_cast<BuildInClass*>(result)->create_var();
	}
	case ESymbol::SFUN:
		ret = parse_function_call(this_namespace);
		break;
	case ESymbol::SPOINTER:
		return parse_var(result);
	case ESymbol::SVAR:
	{
		ret = parse_var(result);
		break;
	}
	default:
	{
		auto target_namespace = kill_namespaces();
		if (target_namespace == nullptr)
			throw Error("unsupported id type");
		ret = _parse_id_wn(target_namespace);
		return ret;
	}
	}
	//std::cout << "This Token:" << token_stream.this_token()->to_string();
	Mer::Assign::AssignType assignment_type = _token_to_assType();
	if (_token_to_assType() == Assign::AssignType::Null)
	{
		return ret;
	}
	token_stream.next();
	auto expr = new Expr();
	auto node = new Assign(assignment_type, ret, id, expr->root());
	expr->tree = nullptr;
	delete expr;
	return node;
}

Mer::ParserNode * Mer::Parser::parse_var(WordRecorder* var_info)
{
	Token *var_id = token_stream.this_token();
	token_stream.match(ID);
	switch (token_stream.this_tag())
	{
	case DOT:
	{
		return parse_member(var_info);
	}
	case LSB:
	{
		token_stream.back();
		return parse_array(var_info);
	}
	default:
		return new Variable(var_info);
	}
}



Mer::ParserNode * Mer::Parser::_parse_id_wn(Namespace * names)
{
	ParserNode *ret = nullptr;
	auto id = token_stream.this_token();
	auto result = names->sl_table->find(Id::get_value(id));
	if (result == nullptr)
		throw Error(id->to_string() + " no found");
	switch (result->es)
	{
		/*
	case ESymbol::SSTRUCTURE:
	{
		ret = build_init_list(names);
		break;
	}*/
	case ESymbol::SFUN:

		ret = parse_function_call(names);
		break;
	case ESymbol::SGVAR:
	{
		ret = parse_glo(result);
		break;
	}
	default:
		throw Error("Unsupported id type");
	}
	Assign::AssignType assignment_type;
	switch (token_stream.this_tag())
	{
	case ASSIGN:
		assignment_type = Assign::AssignType::None;
		break;
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
	token_stream.next();
	return new NVModificationAdapter(assignment_type, ret, new Expr());
}

Mer::FunctionCall * Mer::Parser::parse_function_call(Namespace *names)
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

Mer::Namespace * Mer::Parser::kill_namespaces()
{
	Namespace *current = this_namespace;
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

Mer::LConV::LConV(Token * t)
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
	pos = static_cast<GVarIdRecorder*>(result)->pos+offset;
	type = result->get_type();
}


Mer::ParserNode * Mer::Parser::parse_array(WordRecorder* var_info)
{
	auto id_name= token_stream.this_token();
	token_stream.match(ID);
	token_stream.match(LSB);
	if (token_stream.this_tag() == INTEGER)
	{
		int off = Integer::get_value(token_stream.this_token());
		token_stream.match(INTEGER);
		token_stream.match(RSB);
		if (var_info->get_type() >= USER_TYPE_INDEX)
		{
			if(var_info->es==SGVAR)
				return parse_member_glo(var_info,off);
			return parse_member(var_info,off);
		}
		return new Variable(static_cast<VarIdRecorder*>(var_info)->get_type(), static_cast<VarIdRecorder*>(var_info)->pos+off);
	}
	auto expr = new Expr();
	token_stream.match(RSB);
	if (var_info->get_type() >= USER_TYPE_INDEX)
	{
		throw Error("haven't finished yet");
	}
	return new ContainerIndex(static_cast<VarIdRecorder*>(var_info)->get_type(), static_cast<VarIdRecorder*>(var_info)->pos,expr);
}


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
		if (var_info->get_type() >= USER_TYPE_INDEX)
			return parse_member_glo(var_info, off);
		return new GVar(var_info,  off);
	}
	auto expr = new Expr();
	token_stream.match(RSB);
	return new ContainerIndex(static_cast<VarIdRecorder*>(var_info)->pos, static_cast<VarIdRecorder*>(var_info)->get_type(), expr);
}