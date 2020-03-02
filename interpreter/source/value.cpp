/*
*	MIT License
*	Copyright (c) 2019 Htto Hu
*/
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
#include "../include/environment.hpp"
#include "../include/compound_box.hpp"
namespace Mer {
	namespace optimizer
	{
		ParserNode* optimize_bin_op(ParserNode* left, ParserNode* right, Token* tok);
		ParserNode* optimize_unary_op(ParserNode* left, Token* tok);
		ParserNode* optimize_array_subscript(ParserNode* arr, ParserNode* subscript);
	}
}
Mer::Variable::Variable(WordRecorder* wr)
{
	pos = static_cast<VarIdRecorder*>(wr)->pos;
	type = wr->get_type();
}

type_code_index Mer::Variable::get_type()
{
	return type;
}

size_t Mer::Variable::get_pos() {
	return pos+mem.get_current();
}

Mer::Mem::Object Mer::Variable::execute()
{
	return mem[mem.get_current() + pos];
}

Mer::ParserNode* Mer::Variable::clone()
{
	auto ret = new Variable;
	ret->is_arr = is_arr;
	ret->type = type;
	ret->pos = pos;
	return ret;
}

Mer::FunctionCall::FunctionCall( FunctionBase* _func, const std::vector<ParserNode*>& exprs) : func(_func), argument(exprs)
{
	std::vector<type_code_index> type_vec;
	for (const auto& a : exprs)
	{
		type_vec.push_back(a->get_type());
	}
	func->check_param(type_vec);
	std::vector<ParserNode*> tmp;
}

type_code_index Mer::FunctionCall::get_type()
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
	std::string str = "function:";
	//str += std::to_string(func->funtion_pos);
	str += "(";
	for (auto& a : argument)
		str += a->to_string();
	str += ")";
	return str;
}

Mer::ParserNode* Mer::FunctionCall::clone()
{
	auto ret = new FunctionCall;
	for (auto a : argument)
		ret->argument.push_back(a->clone());
	ret->func = func;
	return ret;
}

Mer::FunctionCall::~FunctionCall()
{
	for (auto a : argument)
		delete a;
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
		return parse_glo(result);
	case ESymbol::SSTRUCTURE:
	case ESymbol::SCONTAINER:
		return var_decl();
	case ESymbol::SFUN:
		return parse_function_call(this_namespace);
	case ESymbol::SGARR:
		return parse_array<GArrayRecorder>(result);
	case ESymbol::SARRAY:
		return parse_array<ArrayRecorder>(result);
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
		auto mv_r = static_cast<MVarRecorder*>(var_info);
		return new MemberVar(mv_r->pos, mv_r->get_type());
	}
	auto ret = new Variable(var_info);
	/*if (var_info->es == SARRAY)
		if (token_stream.this_tag() != LSB)
		{
			std::cout << "CUT";
			delete ret;
			return new ArrayDecay(var_info->get_pos(), var_info->get_type() + 1);
		}
		else
			ret->arr() = true;*/
	return ret;
}

namespace {

	Mer::ParserNode* _make_l_conv(int n) {
		return new Mer::LConV(std::make_shared< Mer::Mem::Int>(n),Mer::Mem::BasicType::INT);
	}
}
template<typename ARR_TYPE>
Mer::ParserNode* Mer::Parser::parse_array(WordRecorder* var_info)
{
	token_stream.match(ID);
	auto array_indexs = static_cast<ARR_TYPE*>(var_info)->array_indexs;
	std::vector<ParserNode*> indexs;
	// if the following of the array is not [, then the array will decay to a pointer which points to the first elements
	if (token_stream.this_tag() != Tag::LSB)
		if(typeid(ARR_TYPE)== typeid(ArrayRecorder))
			return new ArrayDecay(var_info->get_pos(), var_info->get_type() + 1);
		else
			return new GloArrayDecay(var_info->get_pos(), var_info->get_type() + 1);

	while (token_stream.this_tag() == Tag::LSB) {
		token_stream.match(LSB);
		indexs.push_back(Expr().root());
		token_stream.match(RSB);
	}
	// obtain the a index from the indexs
	ParserNode* ret = _make_l_conv(0);
	for (int i = 0; i < indexs.size()-1; i++) {
		int p = 1;
		for (int j = 0; j <= i; j++)
		{
			p *= array_indexs[indexs.size() - 1 - i];
		}
		ParserNode* tmp = optimizer::optimize_bin_op(indexs[i], _make_l_conv(p), BasicToken["*"]);
		ret = optimizer::optimize_bin_op(ret, tmp, BasicToken["+"]);
	}
	ret = optimizer::optimize_bin_op(ret, indexs.back(), BasicToken["+"]);
	if(typeid(ARR_TYPE)==typeid(ArrayRecorder))
		return optimizer::optimize_array_subscript(new Variable(var_info), ret);
	return optimizer::optimize_array_subscript(new GVar(var_info), ret);
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
		throw Error("Unsupported type "+id->to_string());
	}
}

Mer::FunctionCall* Mer::Parser::parse_initializer(type_code_index type)
{
	std::vector<ParserNode*> exprs = parse_arguments();
	// get the args' type to find init function
	// cos the init function can be overloaded by the different param
	std::vector<type_code_index> args_type;
	for (auto& a : exprs)
	{
		args_type.push_back(a->get_type());
	}
	auto result = type_init_function_map.find(InitKey(type,args_type ));

	std::string err_msg="(";
	// get the information of args
	for (size_t i = 0; i < args_type.size(); i++)
	{
		err_msg += type_to_string(args_type[i]);
		if (i != args_type.size() - 1)
			err_msg += ",";
	}
	err_msg += ")";

	if (result == type_init_function_map.end())
		throw Error("type:" + type_to_string(type) + " don't support initializer "+err_msg);
	auto func = result->second;

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

Mer::ParserNode* Mer::Parser::parse_function_call(Namespace* names)
{

	std::string func_name = Id::get_value(token_stream.this_token());
	// to check the param's type.

	token_stream.match(ID);
	std::vector<ParserNode*> exprs = parse_arguments();
	ParamFeature pf;
	for (auto a : exprs)
		pf.push_back(a->get_type());
	auto result = names->sl_table->find(func_name);
	auto recorder = static_cast<FuncIdRecorder*>(result);
	auto func = recorder->find(pf);
	if (func == nullptr)
	{
		throw Error("function " + func_name + param_feature_to_string(pf)+ " no found its defination");
	}
	return new FunctionCall(func, exprs);
}

Mer::MemberFunctionCall* Mer::Parser::parse_call_by_function(FunctionBase* f)
{
	std::vector<ParserNode*> exprs;
	// to check the param's type.
	//get ref of parent
	auto tmp = parse_arguments();
	exprs.insert(exprs.end(), tmp.begin(), tmp.end());
	return new MemberFunctionCall(f, exprs, structure_parent_stack.back());
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

Mer::ParserNode* Mer::LConV::clone()
{
	auto ret = new LConV;
	ret->type = type;
	ret->obj = obj->clone();
	return ret;

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

Mer::MemberFunctionCall::MemberFunctionCall(FunctionBase* _func, std::vector<ParserNode*>& exprs, ParserNode* _p) : parent(_p),func(_func), argument(exprs),obj_vec(exprs.size()+1)
{
	std::vector<type_code_index> type_vec;
	for (auto& a : exprs)
	{
		type_vec.push_back(a->get_type());
	}
	func->check_param(type_vec);
}

type_code_index Mer::MemberFunctionCall::get_type()
{
	return func->get_type();
}

Mer::Mem::Object Mer::MemberFunctionCall::execute()
{
	parents_vec.push_back(parent->execute());
	auto sz = argument.size();
	for (size_t i=0;i<sz;i++)
	{
		obj_vec[i]=argument[i]->execute()->clone();
	}
	auto ret= func->run(obj_vec);
	parents_vec.pop_back();
	return ret;
}

std::string Mer::MemberFunctionCall::to_string()
{
	std::string str = "function:";
	
	str += "(";
	for (auto& a : argument)
		str += a->to_string();
	str += ")";
	return str;
}

Mer::ParserNode* Mer::MemberFunctionCall::clone()
{
	auto ret = new MemberFunctionCall;
	for (auto a : argument)
		ret->argument.push_back(a->clone());
	ret->func = func;
	ret->parent =UptrPNode(parent->clone());
	return ret;
}

Mer::MemberFunctionCall::~MemberFunctionCall()
{
	for (auto a : argument)
		delete a;
}
