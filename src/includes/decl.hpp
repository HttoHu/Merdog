#pragma once
#include "./parser_node.hpp"
#include <vector>

namespace Mer {
	class VarDecl :public ParserNode {
	public:
		using NameT = std::vector<std::string>;
		using PartT = std::vector<std::pair<::size_t, ParserNode*>>;
		// length ,node 
		VarDecl(const NameT& _ids, const PartT& parts, size_t* _base_ptr, ::size_t _pos);
		void execute(char*)override;

		std::string to_string()const override;
		size_t* base_ptr;
		size_t pos;
	private:
		std::vector<std::pair<size_t, ParserNode*>> values;
		std::vector<std::string> ids;
	};
	namespace Parser {
		ParserNode* parse_var_decl();
		void parse_const_decl();
	}
}