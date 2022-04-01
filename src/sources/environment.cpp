#include "../includes/environment.hpp"
#include "../includes/parser_node.hpp"
#include "../includes/memory.hpp"
#include "../includes/lexer.hpp"
#include <vector>


namespace Mer {
	namespace Parser {
		ParserNode* parse_var_decl();
		ParserNode* parse_expr();
	}
	namespace Env {
		Symbol::SymbleTable symbol_table;
	}

	//temporary function...
	std::vector<ParserNode*> parse_block() {
		std::vector<ParserNode*> ins_tab;

		Mem::default_mem.new_block();
		Env::symbol_table.new_block();
		token_stream.match(BEGIN);
		while (token_stream.this_tag() != END)
		{
			ParserNode* node = nullptr;
			switch (token_stream.this_token()->get_tag())
			{
			case CHAR_DECL:
			case BOOL_DECL:
			case INTEGER_DECL:
			case REAL_DECL:
			case STRING_DECL:
				node = Parser::parse_var_decl();
				break;
			default:
				node = Parser::parse_expr();
				break;
			}
			token_stream.match(SEMI);

			ins_tab.push_back(node);
		}
		token_stream.match(END);
		Mem::default_mem.end_block();
		Env::symbol_table.end_block();

		return ins_tab;
	}
	void run_interpreter(const std::string& content)
	{
		Mer::build_token_stream(content);
		auto ins_tab = parse_block();
		for (auto node : ins_tab)
		{
			node->execute(nullptr);
		}
	}
}