#include "../include/namespace.hpp"
#include "../include/value.hpp"
#include "../include/word_record.hpp"
#include "../include/structure.hpp"
using namespace Mer;
Namespace *Mer::root_namespace = new Namespace(nullptr);
Namespace *Mer::this_namespace = root_namespace;
//========================================================================
Mer::Namespace::Namespace(Namespace * pare) :parent(pare)
{
	sl_table = new SymbolTable();
}
/*
void Mer::Namespace::set_new_structure(const std::string & name, Structure * structure)
{
	structures.insert({ name,{structure,type_counter++} });
	sl_table->push_glo(name, new WordRecorder(ESymbol::SSTRUCTURE));
}*/
void Mer::Namespace::set_new_func(const std::string & name, size_t type, FunctionBase * func)
{
	functions.insert({ name,func });
	sl_table->push_glo(name, new FuncIdRecorder(type));
}
void Mer::Namespace::set_new_var(const std::string & name, size_t type, Mem::Object obj)
{
	sl_table->push_glo(name, new GVarIdRecorder(type, obj));
}

Mer::Mem::Object Mer::Namespace::find_var(const std::string & name)
{
	auto result = sl_table->find(name);
	if (result != nullptr)
	{
		if (result->es == ESymbol::SGVAR)
			return static_cast<GVarIdRecorder*>(result)->value;
		else
			throw Error("Error A20");
	}
	if (parent != nullptr)
		return parent->find_var(name);
	else
		throw Error("var " + name + " no found");
}

Mer::FunctionBase * Mer::Namespace::find_func(const std::string & name)
{
	auto result = functions.find(name);
	if (result != functions.end())
		return result->second;
	if (parent != nullptr)
		return parent->find_func(name);
	else
		throw Error("function " + name + " no found");
}

Mer::Namespace * Mer::Parser::_find_namespace_driver(Mer::Namespace *current, const std::string &name)
{
	auto result = current->children.find(name);
	if (result == current->children.end())
	{
		if (current->parent == nullptr)
			return nullptr;
		return _find_namespace_driver(current->parent, name);
	}
	return result->second;
}

Mer::Namespace * Mer::Parser::find_namespace(const std::string & name)
{
	auto result = _find_namespace_driver(this_namespace, name);
	if (result == nullptr)
		return nullptr;
	return result;
}

Mer::Namespace * Mer::Parser::build_namespace() {
	Namespace *nns = new Namespace(this_namespace);
	token_stream.match(NAMESPACE);
	std::string nnname = GIC();
	this_namespace->children.insert({ nnname,nns });
	this_namespace = nns;
	token_stream.match(BEGIN);
	this_namespace->sl_table->new_block();

	while (true)
	{
		switch (token_stream.this_tag())
		{
			/*
		case STRUCT:
			parse_structure();
			break;
		case NAMESPACE:
		{
			build_namespace();
			break;
		}*/
		case FUNCTION:
			build_function();
			break;
		case BOOL_DECL:
		case INTEGER_DECL:
		case REAL_DECL:
		case STRING_DECL:
			create_namespace_var();
			break;
		case END:
			goto e;
		default:
			throw Error("syntax error");
		}
	}
e:
	this_namespace->sl_table->end_block();
	token_stream.match(END);
	this_namespace = nns->parent;
	return nns;
}

void Mer::Parser::create_namespace_var()
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
	for (auto &a : var_list)
	{
		Mem::Object nobj = a.second->execute();
		this_namespace->set_new_var(Id::get_value(a.first), type_code, nobj);
	}
	token_stream.match(SEMI);
}

Mer::NVModificationAdapter::NVModificationAdapter(Assign::AssignType mt, ParserNode * o, Expr * e)
	:mtype(mt), expr(e)
{
	obj = o->execute();
}
