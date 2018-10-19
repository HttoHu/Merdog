#include "../include/memory.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
#include "../include/environment.hpp"
#include "../include/structure.hpp"
Mer::Variable::Variable(Token * tok) :id(tok)
{
	auto result = this_namespace->sl_table->find(Id::get_value(tok));
	if (result == nullptr)
		throw Error("var " + tok->to_string() + " no found");
	if (result->es != ESymbol::SVAR)
		throw Error(tok->to_string() + " must be a symbol");
	pos = static_cast<VarIdRecorder*> (result)->pos;
}

size_t Mer::Variable::get_type()
{
	auto result = this_namespace->sl_table->find(Id::get_value(id));
	if (result == nullptr)
		throw Error("id " + id->to_string() + " no found");
	if (result->es == ESymbol::SVAR)
	{
		return static_cast<VarIdRecorder*>(result)->get_type();
	}
	throw Error(id->to_string() + " is not a var");
}

Mer::Mem::Object Mer::Variable::execute()
{
	return stack_memory[pos];
}

Mer::FunctionCall::FunctionCall(const std::vector<size_t> &types, size_t _index, FunctionBase * _func, std::vector<Expr*>& exprs) :index(_index), func(_func), argument(exprs)
{
	if (!func->check_param(types))
	{
		throw Error( "A02 type not matched");
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
/*
Mer::ParserNode * Mer::Parser::build_init_list(Namespace *names)
{
	std::string type_name = GIC();
	auto result = names->sl_table->find(type_name);
	if (result->es != ESymbol::SSTRUCTURE)
		throw Error("merdog inner errors. 1");
	Structure* structure = static_cast<Structure*>(type_vtable[result->get_type()]);
	token_stream.match(BEGIN);
	std::map<std::string, Expr*> tmp;// to init InitList;
	while (token_stream.this_tag() != END)
	{
		std::string name = GIC();
		token_stream.match(COMMA);
		tmp.insert({ name,new Expr() });
	}
	token_stream.match(END);
	return new InitList(structure, tmp);
}
*/
Mer::ParserNode * Mer::Parser::parse_id()
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
	case ESymbol::STYPE:
	{
		return static_cast<ContainerTypeRecorder*>(result)->create_var();
	}
	case ESymbol::SFUN:
		ret = parse_function_call(this_namespace);
		break;
	case ESymbol::SGVAR:
	{
		ret = _parse_id_wn(Mer::root_namespace);
		return ret;
	}
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
	Mer::Assign::AssignType assignment_type = _token_to_assType();
	if (_token_to_assType() == Assign::AssignType::Null)
		return ret;
	size_t pos = static_cast<VarIdRecorder*>(result)->pos;
	token_stream.next();
	auto expr = new Expr();
	auto node = new Assign(assignment_type, pos, id, expr->root());
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
		// if a type_code less than 7, it must be a basic type (int real bool string) which doesn't have their members.;
		if (var_info->get_type() <= BASICTYPE_MAX_CODE)
		{
			throw Error("basic-type var doesn't have members");
		}
		StructureBase *structure = nullptr;
		{
			// find structure;
			auto result = structure_seeker.find(var_info->get_type());
			if (result == structure_seeker.end())
				throw Error("type no found");
			structure = result->second;
		}
		token_stream.match(DOT);
		ParserNode* tmp = new Variable(var_id);
		return parse_function_call(static_cast<Expr*>(tmp), structure);
	}
	default:
		return new Variable(var_id);
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
		ret = new GVar(names, token_stream.this_token());
		token_stream.match(ID);
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

Mer::FunctionCall * Mer::Parser::parse_function_call(Mer::Expr * co_name, StructureBase *sb)
{
	auto id = token_stream.this_token();
	std::vector<Expr*> exprs{ co_name };
	// to check the param's type.
	std::vector<size_t> param_types;

	auto result = sb->get_function(Id::get_value(id));
	token_stream.match(ID);

	if (result == nullptr)
		throw Error("function " + id->to_string() + " no found its defination");
	token_stream.match(LPAREN);
	if (token_stream.this_tag() == RPAREN)
	{
		token_stream.match(RPAREN);
		return new FunctionCall(param_types, stack_memory.get_index(), result, exprs);
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

	return new FunctionCall(param_types, stack_memory.get_index(), result, exprs);
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
		return new FunctionCall(param_types, stack_memory.get_index(), result, exprs);
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
	return new FunctionCall(param_types,stack_memory.get_index(), result, exprs);
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

Mer::GVar::GVar(Namespace *ns, Token * o)
{
	auto result = ns->sl_table->find(Id::get_value(o));
	if (result == nullptr)
		throw Error(o->to_string() + "no found");
	if (result->es != ESymbol::SGVAR)
		throw Error("merdog inner-errors");
	obj = static_cast<GVarIdRecorder*>(result)->value;
	type = result->get_type();
}
