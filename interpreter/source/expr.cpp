/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/expr.hpp"
#include "../include/value.hpp"
#include "../include/compound_box.hpp"
#include "../include/memory.hpp"
#include "../include/namespace.hpp"
#include "../include/function.hpp"
#include "../include/word_record.hpp"
#include "../include/optimizer.hpp"

namespace Mer
{
	Expr::Expr(type_code_index t) :is_bool(false), expr_type(t) {
		tree = assign();
		if (expr_type == 0 && tree->get_type() != 0)
		{
			expr_type = tree->get_type();
		}
	}
	type_code_index Expr::get_type()
	{
		if (is_bool)
		{
			return Mem::BOOL;
		}
		if (tree == nullptr)
			return Mem::BVOID;
		if (expr_type != 0)
			return expr_type;
		return tree->get_type();
	}
	ParserNode* Expr::assign()
	{
		auto result = and_or();
		while (token_stream.this_tag() == ASSIGN || token_stream.this_tag() == SADD || token_stream.this_tag() == SSUB || token_stream.this_tag() == SMUL || token_stream.this_tag() == SDIV)
		{
			auto tok = token_stream.this_token();
			token_stream.next();
			result = new BinOp(result, tok, and_or());
		}
		return result;
	}
	Mer::ParserNode* Expr::and_or()
	{
		auto result = nexpr();
		while (token_stream.this_tag() == AND || token_stream.this_tag() == OR)
		{
			is_bool = true;
			auto tok = token_stream.this_token();
			token_stream.next();
			result = new LogicalBinOp(result, tok, nexpr());
		}
		return result;

	}

	ParserNode* Expr::expr()
	{
		auto result = term();

		while (token_stream.this_token()->get_tag() == PLUS || token_stream.this_token()->get_tag() == MINUS)
		{
			auto tok = token_stream.this_token();
			token_stream.next();
			result = optimizer::optimize_bin_op(result, term(), tok);
		}
		return result;
	}

	ParserNode* Expr::nexpr()
	{
		auto result = expr();
		while (1)
		{
			auto tok = token_stream.this_token();
			switch (tok->get_tag())
			{
			case EQ:
			case NE:
			case GE:
			case GT:
			case LE:
			case LT:
				is_bool = true;
				token_stream.next();
				break;
			default:
				return result;
			}
			result = optimizer::optimize_bin_op(result, expr(),tok);
		}
		return result;
	}

	ParserNode* Expr::term()
	{
		auto result = member_visit();
		while (token_stream.this_token()->get_tag() == MUL || token_stream.this_token()->get_tag() == DIV)
		{
			auto tok = token_stream.this_token();
			token_stream.next();
			result = optimizer::optimize_bin_op(result, member_visit(),tok);
		}
		return result;
	}

	ParserNode* Expr::member_visit()
	{
		auto result = subscript();
		while (token_stream.this_token()->get_tag() == DOT || token_stream.this_token()->get_tag() == PTRVISIT)
		{
			auto tok = token_stream.this_token();
			token_stream.next();
			auto member_id = token_stream.this_token();
			token_stream.match(ID);
			type_code_index type_code = result->get_type();
			// get rid of the pointness.
			type_code_index raw_type = type_code + (type_code % 2 - 1);

			// member function call
			if (token_stream.this_tag() == LPAREN)
			{
				structure_parent_stack.push_back(result);
				result = nullptr;
				// get type code
				auto func = member_function_table[raw_type].find(Id::get_value(member_id));
				if (func == member_function_table[raw_type].end())
				{ 
					throw Error(" member functioon " + Id::get_value(member_id) + " no found");
				}
				result= Parser::parse_call_by_function(func->second);
				structure_parent_stack.pop_back();
				continue;
			}
			// find struct info
			auto bias = type_code+ (type_code % 2 - 1);
			auto ustruct = find_ustructure_t(bias);
			// find member index and type;
			auto seeker = ustruct->get_member_info(Id::get_value(member_id));
			result=new Index(result, seeker.second, seeker.first);
		}
		return result;
	}
	ParserNode* Expr::subscript()
	{
		auto result = factor();
		while (token_stream.this_token()->get_tag() == LSB)
		{
			auto tok = token_stream.this_token();
			token_stream.match(LSB);
			auto exp = expr();
			token_stream.match(RSB);
			result = optimizer::optimize_array_subscript(result, exp);
		}
		return result;
	}
	ParserNode* Expr::factor()
	{
		auto result = token_stream.this_token();
		switch (result->get_tag())
		{
		case MAKE:
			return Parser::make_var();
		case NEW:
			return new NewExpr();
		case MUL:
			return new RmRef();
		case GET_ADD:
			return new GetAdd();
		case BEGIN:
		{
			auto result = find_ustructure_t(expr_type);
			return new StructureInitList(result,expr_type);
		}
		case CAST:
			return new Cast();
		case TTRUE:
		{
			token_stream.match(TTRUE);
			return new LConV(result);
		}
		case TFALSE:
		{
			token_stream.match(TFALSE);
			return new LConV(result);
		}
		case LPAREN:
		{
			token_stream.match(LPAREN);
			ParserNode* v = assign();
			token_stream.match(RPAREN);
			return v;
		}
		case CHAR_LIT:
		case REAL:
		case STRING:
		case INTEGER:
			token_stream.next();
			return new LConV(result);
		case PLUS:
		case NOT:
		case MINUS:
		{
			token_stream.next();
			ParserNode* n = optimizer::optimize_unary_op(member_visit(),result);
			return n;
		}
		case SIZEOF:
			return new SizeOf();
		case ID:
			return Parser::parse_id();
		case NULLPTR:
			token_stream.match(NULLPTR);
			return new LConV(std::make_shared<Mem::Pointer>(nullptr), expr_type);
		default:
			return new NonOp();
		}
	}

	BinOp::BinOp(ParserNode* l, Token* o, ParserNode* r) :left(l), right(r),op_tok(o)
	{
		auto result = optimizer::op_table.find(o->get_tag());
		if (result == optimizer::op_table.end())
			throw Error(o->to_string() + " invalid operation");
		op = result->second;
		if (o->get_tag() != LSB && l->get_type() != r->get_type())
		{
			if (!(l->get_type() % 2 && r->get_type() % 2))
				return;
			right = new Cast(r, l->get_type());
		}
	}

	Mem::Object BinOp::execute()
	{
		return op(left->execute(), right->execute());
	}

	type_code_index BinOp::get_type()
	{
		switch (op_tok->get_tag())
		{
		case AND:
		case OR:
		case EQ:
		case NE:
		case GE:
		case GT:
		case LE:
		case LT:
			return Mem::BOOL;
		default:
			break;
		}
		return left->get_type();
	}

	std::string BinOp::to_string()
	{
		return left->to_string() + op_tok->to_string() + right->to_string();
	}

	Mer::Mem::Object UnaryOp::execute()
	{
		switch (op->get_tag())
		{
		case NOT:
		case MINUS:
		{
			auto tmp = expr->execute();
			auto ret = tmp->get_negation();
			return Mem::Object(ret);
		}
		case PLUS:
			return expr->execute();
		default:
			throw Error("no matched operator");
		}
	}


	InitList::InitList(type_code_index t):type(t)
	{
		token_stream.match(BEGIN);
		while (token_stream.this_tag() != Tag::END)
		{
			auto insertion = Expr().root();
			if (insertion->get_type() != t)
			{ 
				size_t insertion_type = insertion->get_type();
				delete insertion;
				throw Error("init_list type not matched from " + type_to_string(t) + " to " + type_to_string(insertion_type));
			}
			init_v.push_back(insertion);
			if (token_stream.this_tag() == Tag::COMMA)
				token_stream.match(COMMA);
		}
		token_stream.match(END);
	}

	InitList::InitList(type_code_index t, int sz) :type(t), size(sz)
	{
		token_stream.match(BEGIN);
		while (token_stream.this_tag() != Tag::END)
		{
			init_v.push_back(Expr(t).root());
			if (token_stream.this_tag() == Tag::COMMA)
				token_stream.match(COMMA);
		}
		token_stream.match(END);
		if (init_v.size() == 1 && sz > 1)
		{
			for (int i = 1; i < sz; i++)
			{
				init_v.push_back(init_v[0]->clone());
			}
			return;
		}
		if (sz == -1)
			size = init_v.size();
		else if (init_v.size() != sz)
			throw Error("Error, array overflow expect " + std::to_string(sz) + " but received " + std::to_string(init_v.size()));

		for (size_t i = 1; i < init_v.size(); i++)
		{
			if (type != init_v[i - 1]->get_type())
			{
				throw Error("there is a type-distinction in an init list. from "+ type_to_string(type)+" to "+type_to_string(init_v[i-1]->get_type()));
			}
		}

	}

	Mem::Object InitList::execute()
	{
		std::vector<Mem::Object> v(init_v.size());
		auto sz = init_v.size();
		for (size_t i = 0; i < sz; i++)
		{
			 v[i]=init_v[i]->execute()->clone();
		}
		auto ret= std::make_shared<Mem::InitListObj>(std::move(v), type);
		return ret;
	}

	std::vector<Mem::Object> InitList::get_array()
	{
		std::vector<Mem::Object> v(init_v.size());
		if (v.size() == 1 && size > 1)
		{
			auto tmp = init_v[0]->execute();
			v = std::vector<Mem::Object>(size, tmp);
			return v;
		}
		if (v.size() != size)
			throw Error("list size is not matched with array");
		for (size_t i = 0; i < v.size(); i++)
		{
			v[i] = init_v[i]->execute();
		}
		return v;
	}

	ParserNode* InitList::clone()
	{
		InitList* ret = new InitList;
		ret->type = type;
		ret->size = size;
		for (auto& a : init_v)
			ret->init_v.push_back(a->clone());
		return ret;
	}


	Mem::Object ContainerIndex::execute()
	{
		auto tmp = expr->execute();
		return mem[mem.get_current() + pos + std::static_pointer_cast<Mem::Int>(tmp)->get_value()];
	}

	type_code_index ContainerIndex::get_type()
	{
		return type;
	}

	ContainerIndex::~ContainerIndex()
	{
		 delete expr; 
	}

	EmptyList::EmptyList(type_code_index t, size_t sz) :type_code(t), size(sz)
	{
		for (size_t i = 0; i < sz; i++)
		{
			init_v.push_back(new LConV(Mem::create_var_t(t), t));
		}
	}

	Mem::Object EmptyList::execute()
	{
		return nullptr;
	}

	size_t ContainerGloIndex::get_pos()
	{
		size_t off_set = Mem::get_raw<int>(expr->execute());
		return off_set + pos;
	}

	Mem::Object ContainerGloIndex::execute()
	{
		auto tmp = expr->execute();
		return mem[pos + std::static_pointer_cast<Mem::Int>(tmp)->get_value()];
	}

	NewExpr::NewExpr(bool init_nothing)
	{
		if (init_nothing)
			return;
		token_stream.match(NEW);
		type_code_index type_code = Mem::get_type_code();
		if (token_stream.this_tag() == LPAREN)
		{
			expr = Expr(type_code).root();
		}
		else if (token_stream.this_tag() == BEGIN)
		{
			auto _result = find_ustructure_t(type_code);
			expr = new StructureInitList(_result, type_code);
		}
		else if (token_stream.this_tag() == SEMI)
		{
			auto insertion = Mem::create_var_t(type_code);
			expr = new LConV(insertion, type_code);
		}
		if (expr->get_type() != type_code)
		{
			throw Error("new-type not matched from " + type_to_string(expr->get_type()) + " to " + type_to_string(type_code));
		}
	}

	ParserNode* NewExpr::clone()
	{
		NewExpr* ret = new NewExpr(true);
		ret->expr = expr->clone();
		return ret;
	}

	Mem::Object NewExpr::execute()
	{
		return std::make_shared<Mem::Pointer>(expr->execute()->clone());
	}

	GetAdd::GetAdd()
	{
		token_stream.match(GET_ADD);
		id = Expr().root();
		type = id->get_type();
	}

	type_code_index Mer::GetAdd::get_type()
	{
		return type + 1;
	}

	Mem::Object Mer::GetAdd::execute()
	{
		return std::make_shared<Mem::Pointer>(id->execute());
	}

	RmRef::RmRef(bool init_nothing)
	{
		if (init_nothing)
			return;
		token_stream.match(MUL);
		id = Parser::parse_id();
		type = id->get_type();
	}

	type_code_index Mer::RmRef::get_type()
	{
		return type - 1;
	}
	Mem::Object Mer::RmRef::execute()
	{

		return std::static_pointer_cast<Mem::Pointer>(id->execute())->rm_ref();
	}
	ParserNode* RmRef::clone()
	{
		RmRef* ret = new RmRef(true);
		ret->id = id->clone();
		ret->type = type;
		return ret;
	}
	Mer::Index::Index(ParserNode* l, size_t _index, type_code_index _type) :left(std::move(l)), index(_index), type(_type)
	{
		if (_type == -1)
			type = left->get_type();
	}
	Mem::Object Index::execute()
	{
		return left->execute()->operator[](std::make_shared<Mem::Int>(index));
	}

	type_code_index Mer::Index::get_type()
	{
		return type;
	}
	SubScript::SubScript(ParserNode* l, ParserNode* s) :left(l), subscr(s)
	{
		if (Mem::exist_operator(l->get_type(), "[]"))
		{ 
			type = Mem::find_op_type(l->get_type(), "[]");
		}
		else
			type = l->get_type();
	}
	Mem::Object SubScript::execute()
	{
		return left->execute()->operator[](subscr->execute());
	}

	LogicalBinOp::LogicalBinOp(ParserNode* l, Token* tok, ParserNode* r) :left(l),right(r)
	{
		if (tok->get_tag() == AND)
			is_and_op = true;
		else if (tok->get_tag() == OR)
			is_and_op = false;
		else
			throw Error("intern error");
	}

	Mem::Object LogicalBinOp::execute()
	{
		if (is_and_op ^ Mem::get_raw<bool>(left->execute()))
			return std::make_shared<Mem::Bool>(!is_and_op);
		return std::make_shared<Mem::Bool>(Mem::get_raw<bool>(right->execute()));
	}

	Mem::Object ArrayDecay::execute() {
		return std::make_shared<Mem::Pointer>(mem[pos + mem.get_current()+1]);
	}

	Mem::Object GloArrayDecay::execute()
	{
		return std::make_shared<Mem::Pointer>(mem[pos+1]);
	}

}