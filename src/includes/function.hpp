#pragma once
#include "./parser_node.hpp"
#include <vector>

namespace Mer {
	using ParamFeature = std::vector<type_code_index>;

	class Param {
	public:
		bool type_check(const std::vector<type_code_index>& types);
		std::vector<std::pair<type_code_index, size_t>>& get_param_table() { return arg_pos; }
		ParamFeature get_param_feature();
		void push_new_param(type_code_index type, size_t pos)
		{
			arg_pos.push_back({ type,pos });
		}
	private:
		size_t param_size;
		std::vector<std::pair<type_code_index, size_t>> arg_pos;
	};
	class FunctionBase
	{
	public:
		FunctionBase() {}
		virtual type_code_index get_type() { return 0; }
		void run(char* mem) { return; }
		std::string to_string()const;
		virtual ~FunctionBase();
		std::vector<type_code_index> param_types;
	private:
		
		size_t params_count;
	};

	class Function :public FunctionBase {
	public:
		Function(type_code_index t, Param* p);

		Function(type_code_index t);

		Param* param = nullptr;

		void run(char* mem);

		std::vector<ParserNode*> stmts;

		size_t* pc = new size_t(0);
		size_t off = 0;
		~Function();
	private:
		size_t param_size = 0;
		type_code_index type;
	};
	namespace Parser {
		void parse_function();
	}
}