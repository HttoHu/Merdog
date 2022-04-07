#include "../includes/environment.hpp"
#include "../includes/parser_node.hpp"
#include "../includes/memory.hpp"
#include "../includes/lexer.hpp"
#include <vector>


namespace Mer {
	namespace Parser {
		ParserNode* parse_var_decl();
		ParserNode* parse_expr();
		void parse_const_decl();
		void public_part(bool is_single = false);
	}
	namespace Env {
		Symbol::SymbleTable* symbol_table=nullptr;
		std::vector<UptrPNode>* cur_ins_table = nullptr;
		size_t* cur_pc;
		std::vector<std::pair<PosPtr, PosPtr>> nearest_loop_pos;
		bool skip_block_begin;


		void new_loop(PosPtr start, PosPtr end)
		{
			nearest_loop_pos.push_back({ start,end });
		}
		void end_loop()
		{
			if (nearest_loop_pos.empty())
				throw Error("invalid loop scope");
			nearest_loop_pos.pop_back();
		}
		PosPtr loop_start()
		{
			if (nearest_loop_pos.empty())
				throw Error("invalid loop scope");
			return nearest_loop_pos.back().first;
		}
		PosPtr loop_end()
		{
			if (nearest_loop_pos.empty())
				throw Error("invalid loop scope");
			return nearest_loop_pos.back().second;
		}
		void new_block()
		{
			Mem::default_mem.new_block();
			symbol_table->new_block();
		}
		void end_block()
		{
			Mem::default_mem.end_block();
			symbol_table->end_block();
		}
	}
	void init_interpreter() {
		Env::symbol_table = new Symbol::SymbleTable();
		Env::cur_ins_table = new std::vector<UptrPNode>();
		Env::cur_pc = new size_t(0);
	}
	void run_interpreter(const std::string& content)
	{
		init_interpreter();

		Mer::build_token_stream(content);
		time_t s = clock();
		//Mer::token_stream.print();
		token_stream.match(BEGIN);
		Env::new_block();
		Parser::public_part();
		Env::end_block();

		auto &ins = *Env::cur_ins_table;
		int idx = 0;
		for (auto& i : ins)
		{
			std::cout <<idx++<<" : "<< i->to_string() << std::endl;
		}
		size_t& pc = *Env::cur_pc;
		while (pc < ins.size())
		{
			ins[pc]->execute(nullptr);
			pc++;
		}
		time_t e = clock();
		std::cout << "Time: " << 1000 * (e - s) / CLK_TCK <<" ms";
	}
}