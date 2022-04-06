#include "../includes/branch_loop.hpp"
#include "../includes/lexer.hpp"
#include "../includes/environment.hpp"
#include "../includes/memory.hpp"
#include "../includes/expr.hpp"


namespace Mer
{
	namespace BranchAndLoop
	{
		void Goto::execute(char*)
		{
			*pc = *dest - 1;
		}
		std::string Goto::to_string() const
		{
			return "(goto " + std::to_string(*dest) + ")";
		}
		void Continue::execute(char*)
		{
			*pc = *dest - 2;
		}
		void IfFalseJmp::execute(char* dest)
		{
			//std::cout << "----" << expr->to_string() << "\n";
			//std::cout << (static_cast<Expr*>(expr)->get_bool_value()) << "\n";
			if (!(static_cast<Expr*>(expr)->get_bool_value()))
			{
				*pc = *false_tag - 1;
			}
		}
		std::string IfFalseJmp::to_string() const
		{
			return "(if " + expr->to_string() + ", false jmp " + std::to_string(*false_tag) + ")";
		}
	}

	namespace Parser {
		ParserNode* statement();
		ParserNode* parse_expr();

		// {stmts } or stmts
		void parse_auto_block_part() {
			if (token_stream.this_tag() == BEGIN)
			{
				token_stream.match(BEGIN);
				public_part();
				token_stream.match(END);
			}
			else
				public_part(true);
		}

		void build_block()
		{
			if (!Env::skip_block_begin)
			{
				Mem::default_mem.new_block();
				Env::symbol_table->new_block();
			}
			else
				Env::skip_block_begin = false;

			token_stream.match(BEGIN);
			public_part();
			token_stream.match(END);
			Mem::default_mem.end_block();
			Env::symbol_table->end_block();
		}

		void public_part(bool is_single)
		{
			using namespace BranchAndLoop;
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
					Env::cur_ins_table->push_back(std::make_unique<Goto>(Env::cur_pc, Env::loop_end()));
					break;
				case CONTINUE:
					token_stream.match(CONTINUE);
					token_stream.match(SEMI);
					Env::cur_ins_table->push_back(std::make_unique<Continue>(Env::cur_pc, Env::loop_end()));
					break;
				case FOR:
					build_for();
					break;
				case WHILE:
					build_while();
					break;
				case DO:
					do_while();
					break;
				default:
					Env::cur_ins_table->push_back(std::unique_ptr<ParserNode>(statement()));
					break;
				}
				if (is_single)
					break;
			}
		}
		void build_for()
		{
		}
		void do_while()
		{
		}
		void build_while()
		{
		}
		void build_if()
		{
			using namespace BranchAndLoop;

			token_stream.match(IF);
			token_stream.match(LPAREN);

			PosPtr end_pos = std::make_shared<size_t>(0);
			PosPtr end_if_block = std::make_shared<size_t>(0);

			auto condition = parse_expr();
			token_stream.match(RPAREN);

			Env::new_block();
			auto branch = new IfFalseJmp(Env::cur_pc, condition);
			Env::cur_ins_table->push_back(UptrPNode(branch));
			parse_auto_block_part();

			Env::end_block();
			if (token_stream.this_tag() == ELSE)
			{
				token_stream.match(ELSE);
				auto goto_end = new Goto(Env::cur_pc, end_pos);
				Env::cur_ins_table->push_back(UptrPNode(goto_end));

				*end_if_block = Env::cur_ins_table->size();
				branch->false_tag = end_if_block;

				Env::new_block();
				parse_auto_block_part();
				Env::end_block();

				*end_pos = Env::cur_ins_table->size();
				goto_end->dest = end_pos;
			}
			else {
				*end_pos = Env::cur_ins_table->size();
				branch->false_tag = end_pos;
			}
		}
	}
}


