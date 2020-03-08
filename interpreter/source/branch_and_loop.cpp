/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/branch_and_loop.hpp"
#include "../include/namespace.hpp"
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
#include <ctime>
#include "../include/environment.hpp"
namespace Mer
{
	size_t current_function_rety = 0;
	std::vector<UptrPNode>* current_ins_table = nullptr;
	PosPtr this_block_size = nullptr;
	// the postion of the instruction
	extern std::vector<size_t*> _pcs;
	Mem::Object function_ret = nullptr;
	namespace Parser
	{
		void switch_driver();
		void build_for();
		void public_part()
		{
			while (token_stream.this_tag() != END)
			{
				switch (token_stream.this_tag())
				{
				case IF:
					build_if();
					break;
				case BREAK:
					token_stream.match(BREAK);
					token_stream.match(SEMI);
					current_ins_table->push_back(std::make_unique<Goto>(_pcs.back(), loop_end()));
					break;
				case CONTINUE:
					token_stream.match(CONTINUE);
					token_stream.match(SEMI);
					current_ins_table->push_back(std::make_unique<Continue>(_pcs.back(), loop_end()));
					break;
				case RETURN:
					token_stream.match(RETURN);
					current_ins_table->push_back(std::make_unique<Return>(_pcs.back(), Expr().root()));
					token_stream.match(SEMI);
					break;
				case FOR:
					build_for();
					break;
				case SWITCH:
					switch_driver();
					break;
				case WHILE:
					build_while();
					break;
				case DO:
					do_while();
					break;
				default:
					current_ins_table->push_back(std::unique_ptr<ParserNode>(statement()));
					break;
				}
			}
		}
		void build_block()
		{
			mem.new_block();
			this_namespace->sl_table->new_block();
			this_block_size = std::make_shared<size_t>(0);
			token_stream.match(BEGIN);
			public_part();
			*this_block_size = current_ins_table->size();
			token_stream.match(END);
			mem.end_block();
			this_namespace->sl_table->end_block();

		}
		void build_function_block()
		{
			this_block_size = std::make_shared<size_t>(0);
			token_stream.match(BEGIN);
			public_part();
			*this_block_size = current_ins_table->size();
			token_stream.match(END);
			mem.end_block();
			this_namespace->sl_table->end_block();
		}
		void do_while()
		{
			PosPtr start_pos = std::make_shared<size_t>(current_ins_table->size());
			PosPtr end_pos = std::make_shared<size_t>(0);
			// register the start_pos and end_pos to environment
			new_loop(start_pos, end_pos);
			token_stream.match(DO);
			// new block
			this_namespace->sl_table->new_block();
			token_stream.match(BEGIN);
			public_part();
			token_stream.match(END);
			token_stream.match(WHILE);
			token_stream.match(LPAREN);
			Expr* node = new Expr();
			token_stream.match(RPAREN);
			current_ins_table->push_back(std::make_unique<IfTrueToAOrB>(_pcs.back(), std::make_shared<size_t>(*start_pos), std::make_shared<size_t>(current_ins_table->size() + 1), node));
			*end_pos = current_ins_table->size();
			// delete the record of start_pos and end_pos, cause the two var is thoroughly used.
			end_loop();
			// end_block;
			mem.end_block();
			this_namespace->sl_table->end_block();
			token_stream.match(SEMI);
		}
		void build_while()
		{
			PosPtr start_pos = std::make_shared<size_t>(current_ins_table->size());
			PosPtr end_pos = std::make_shared<size_t>(0);
			// register the start_pos and end_pos to environment
			new_loop(start_pos, end_pos);
			token_stream.match(WHILE);
			token_stream.match(LPAREN);
			Expr* node = new Expr();
			token_stream.match(RPAREN);
			// new block
			mem.new_block();
			this_namespace->sl_table->new_block();
			token_stream.match(BEGIN);
			current_ins_table->push_back(std::make_unique<IfTrueToAOrB>(_pcs.back(), std::make_shared<size_t>(*start_pos + 1), end_pos, node));
			public_part();
			current_ins_table->push_back(std::make_unique<Goto>(_pcs.back(), start_pos));
			token_stream.match(END);
			*end_pos = current_ins_table->size();
			// end_block, block;
			end_loop();
			mem.end_block();
			this_namespace->sl_table->end_block();
		}
		void build_if()
		{
			bool have_else=false;
			token_stream.match(IF);
			token_stream.match(LPAREN);
			PosPtr end_pos = std::make_shared<size_t>(0);
			auto node = std::unique_ptr<ParserNode>(Expr().root());
			token_stream.match(RPAREN);
			// new block
			mem.new_block();
			this_namespace->sl_table->new_block();

			token_stream.match(BEGIN);
			auto iwjt = new IfWithJmpTable(_pcs.back());
			iwjt->end_pos = end_pos;
			current_ins_table->push_back(std::unique_ptr<IfWithJmpTable>(iwjt));
			iwjt->jmp_table.push_back({ std::move(node),std::make_shared<size_t>(current_ins_table->size()) });
			public_part();
			current_ins_table->push_back(std::make_unique<Goto>(_pcs.back(), end_pos));
			// end_block;
			token_stream.match(END);
			mem.end_block();
			this_namespace->sl_table->end_block();

			while (token_stream.this_tag() == ELSE_IF)
			{
				token_stream.match(ELSE_IF);
				token_stream.match(LPAREN);
				auto expr = Expr().root();
				token_stream.match(RPAREN);
				mem.new_block();
				this_namespace->sl_table->new_block();
				token_stream.match(BEGIN);
				iwjt->jmp_table.push_back({ std::unique_ptr<ParserNode>(expr),std::make_shared<size_t>(current_ins_table->size()) });
				public_part();
				token_stream.match(END);
				current_ins_table->push_back(std::make_unique<Goto>(_pcs.back(), end_pos));
				mem.end_block();
				this_namespace->sl_table->end_block();
			}
			if (token_stream.this_tag() == ELSE)
			{
				have_else = true;
				iwjt->jmp_table.push_back({ std::make_unique <LConV>(std::make_shared<Mem::Bool>(true), (size_t)Mem::BOOL), std::make_shared<size_t>(current_ins_table->size()) });
				token_stream.match(ELSE);
				mem.new_block();
				this_namespace->sl_table->new_block();
				token_stream.match(BEGIN);
				public_part();
				token_stream.match(END);
				mem.end_block();
				this_namespace->sl_table->end_block();
			}
			if(!have_else)
				iwjt->jmp_table.push_back({ std::make_unique <LConV>(std::make_shared<Mem::Bool>(true), (size_t)Mem::BOOL), end_pos});
			*end_pos = current_ins_table->size();
		}
		template<typename KeyType>
		void build_switch(PosPtr& default_pos, std::map<KeyType, PosPtr>& case_set);
		void switch_driver() {
			token_stream.match(SWITCH);
			token_stream.match(LPAREN);
			auto expr =Expr().root();
			token_stream.match(RPAREN);
			size_t type = expr->get_type();
			ParserNode* node;
			if (type == Mem::INT)
				node = new IntCaseSet(_pcs.back(), expr);
			else if (type == Mem::STRING)
				node = new StrCaseSet(_pcs.back(), expr);
			else if(type==Mem::CHAR)
				node = new CharCaseSet(_pcs.back(), expr);
			else
				throw Error("Unsupported case type!");
			current_ins_table->push_back(std::unique_ptr<ParserNode>(node));
			switch (type)
			{
			case Mem::BasicType::CHAR:
				build_switch<char>(static_cast<CharCaseSet*>(node)->default_pos, static_cast<CharCaseSet*>(node)->jmp_table);
				break;
			case Mem::BasicType::INT:
				build_switch<int>(static_cast<IntCaseSet*>(node)->default_pos, static_cast<IntCaseSet*>(node)->jmp_table);
				break;
			case Mem::BasicType::STRING:
				build_switch<std::string>(static_cast<StrCaseSet*>(node)->default_pos, static_cast<StrCaseSet*>(node)->jmp_table);
				break;
			default:
				throw Error("unsupported type to switch");
			}
		}
		template<typename KeyType>
		void build_switch(PosPtr& default_pos, std::map<KeyType, PosPtr>& case_set) {
			PosPtr end_pos = std::make_shared<size_t>(0);
			default_pos = end_pos;
			token_stream.match(BEGIN);
			mem.new_block();
			this_namespace->sl_table->new_block();
			while (token_stream.this_tag() != END)
			{
				switch (token_stream.this_tag())
				{
				case DEFAULT:
					token_stream.match(DEFAULT);
					token_stream.match(COLON);
					default_pos = std::make_shared<size_t>(current_ins_table->size());
					break;
				case CASE:
					token_stream.match(CASE);
					if (typeid(KeyType) == typeid(int))
					{
						int v = Mer::Integer::get_value(token_stream.this_token());
						case_set.insert({ *(KeyType*)(&v),std::make_shared<size_t>(current_ins_table->size() - 1) });
					}
					else if (typeid(KeyType) == typeid(std::string))
					{
						std::string v = Mer::String::get_value(token_stream.this_token());
						case_set.insert({ *(KeyType*)(&v),std::make_shared<size_t>(current_ins_table->size() - 1) });
					}
					else if (typeid(KeyType) == typeid(char))
					{
						char v = Mer::CharToken::get_value(token_stream.this_token());
						case_set.insert({ *(KeyType*)(&v),std::make_shared<size_t>(current_ins_table->size() - 1) });
					}
					else
						throw Error("unsupported case tag");
					token_stream.next();
					token_stream.match(COLON);
					continue;
				case DO:
					do_while();
					break;
				case WHILE:
					build_while();
					break;
				case SWITCH:
					switch_driver();
					break;
				case FOR:
					build_for();
					break;
				case IF:
					build_if();
					break;
				case BREAK:
					token_stream.match(BREAK);
					token_stream.match(SEMI);
					current_ins_table->push_back(std::make_unique<Goto>(_pcs.back(), end_pos));
					break;
				case CONTINUE:
					token_stream.match(CONTINUE);
					token_stream.match(SEMI);
					current_ins_table->push_back(std::make_unique <Continue>(_pcs.back(), loop_end()));
					break;
				case RETURN:
					token_stream.match(RETURN);
					token_stream.match(SEMI);
					current_ins_table->push_back(std::make_unique <Return>(_pcs.back(), new Expr()));
					break;
				default:
					current_ins_table->push_back(std::unique_ptr <ParserNode>(statement()));
					break;
				}
			}
			token_stream.match(END);
			mem.end_block();
			this_namespace->sl_table->end_block();
			*end_pos = current_ins_table->size();
		}
		/*
			var_part;
			goto t;
		start	    step expr;
		t	if.... x:y
			node;
			node;
			goto start;

		*/
		void build_for()
		{
			PosPtr start_pos = std::make_shared<size_t>(current_ins_table->size());

			PosPtr end_pos = std::make_shared<size_t>(0);
			// register the start_pos and end_pos to environment
			ParserNode* compare_part;
			// new block
			this_namespace->sl_table->new_block();
			mem.new_block();
			new_loop(start_pos, end_pos);
			token_stream.match(FOR);
			token_stream.match(LPAREN);
			if (token_stream.this_tag() != SEMI)
			{
				current_ins_table->push_back(std::unique_ptr<ParserNode>(var_decl()));
				++* start_pos;
			}
			token_stream.match(SEMI);
			if (token_stream.this_tag() == SEMI)
			{
				compare_part = new LConV(std::make_shared<Mem::Bool>(true), Mem::BOOL);
			}
			else
			{
				compare_part = Expr().root();
			}
			token_stream.match(SEMI);
			if (token_stream.this_tag() != RPAREN)
			{
				++* start_pos;
				current_ins_table->push_back(std::make_unique <Goto>(_pcs.back(), std::make_shared<size_t>(*start_pos + 1)));
				// start pos is step
				current_ins_table->push_back(std::unique_ptr<ParserNode>(Expr().root()));
			}
			token_stream.match(RPAREN);

			token_stream.match(BEGIN);

			current_ins_table->push_back(std::make_unique <IfTrueToAOrB>(_pcs.back(), std::make_shared<size_t>(current_ins_table->size() + 1), end_pos, compare_part));
			public_part();
			current_ins_table->push_back(std::make_unique <Goto>(_pcs.back(), start_pos));
			token_stream.match(END);
			*end_pos = current_ins_table->size();
			// end_block, block;
			end_loop();
			mem.end_block();
			this_namespace->sl_table->end_block();
		}
	}
	Mem::Object IfTrueToAOrB::execute()
	{
		if (std::static_pointer_cast<Mem::Bool>(expr->execute())->_value())
			*pc = *true_tag - 1;
		else
			*pc = *false_tag - 1;
		return nullptr;
	}
	std::string IfTrueToAOrB::to_string()
	{
		return "if " + expr->to_string() + " is true goto " + std::to_string(1+*true_tag) + " or " + std::to_string(1+*false_tag);
	}
	Mem::Object IfWithJmpTable::execute()
	{
		for (auto& a : jmp_table)
		{
			if (std::static_pointer_cast<Mem::Bool>(a.first->execute())->_value())
			{
				*pc = *a.second - 1;
				return nullptr;
			}
		}
		return nullptr;
	}
	std::string IfWithJmpTable::to_string()
	{
		std::string ret = "if-else";
		for (auto& a : jmp_table)
		{
			ret += "(";
			ret += a.first->to_string();
			ret += " : ";
			ret += std::to_string(*a.second);
			ret += ")-";
		}
		return ret;
	}

	Mem::Object IntCaseSet::execute()
	{
		auto result = jmp_table.find(Mem::get_raw<int>(expr->execute()));
		if (result == jmp_table.end())
			* pc = *default_pos -1;
		else
			*pc = *(result->second);
		return nullptr;
	}
	std::string IntCaseSet::to_string()
	{
		std::string ret = "switch  ";
		for (auto a : jmp_table)
		{
			ret += std::to_string(a.first);
			ret += ":";
			ret += std::to_string(*a.second);
			ret += "   ";
		}
		ret += " default pos: " + std::to_string(*default_pos);
		return ret;
	}
	std::string Goto::to_string()
	{
		return "goto " + std::to_string(1+*target);
	}
	Mem::Object Goto::execute()
	{
		*index = *target - 1;
		return nullptr;
	}
	Mem::Object Continue::execute()
	{
		*index = *target - 2;
		return nullptr;
	}
	std::string Continue::to_string()
	{
		return "continue " + std::to_string(*target - 1);
	}
	Mem::Object StrCaseSet::execute()
	{
		auto result = jmp_table.find(Mem::get_raw<std::string>(expr->execute()));
		if (result == jmp_table.end())
			* pc = *default_pos -1;
		else
			*pc = *(result->second);
		return nullptr;
	}
	std::string StrCaseSet::to_string()
	{
		std::string ret = "switch  ";
		for (auto a : jmp_table)
		{
			ret += a.first;
			ret += ":";
			ret += std::to_string(*a.second);
			ret += "   ";
		}
		ret += " default pos: " + std::to_string(*default_pos);
		return ret;
	}
	Return::Return(size_t* _pc, ParserNode* _expr) :pc(_pc), expr(_expr)
	{
		if (current_function_rety != expr->get_type())
			throw Error("return type not matched with function return type");
		des = this_block_size;
	}
	Mem::Object Return::execute()
	{
		function_ret = expr->execute();
		*pc = *des;
		return nullptr;
	}
	Return::~Return()
	{
		delete expr;
	}
	Mem::Object CharCaseSet::execute()
	{
		auto result = jmp_table.find(std::static_pointer_cast<Mem::Char>(expr->execute())->get_value());
		if (result == jmp_table.end())
			* pc = *default_pos - 1;
		else
			*pc = *(result->second);
		return nullptr;
	}
}