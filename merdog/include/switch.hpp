/*
*	MIT License
*	Copyright (c) 2019 Htto Hu
*/
#pragma once
#include "parser_node.hpp"
namespace Mer
{
	class Expr;
	template<typename KeyType>
	class Switch :public ParserNode
	{
	public:
		Switch(Expr* _expr);
		Mem::Object execute()override;
	private:
		std::map<KeyType, size_t> tag_map;
		std::vector<ParserNode*> ins_table;
		Expr* expr;
		size_t ins_index = 0;
		size_t default_line;
	};
	class Break :public ParserNode
	{
	public:
		Break(size_t* p) :ptr(p) {}
		void set_end_pos(size_t pos)
		{
			end_pos = pos;
		}
		Mem::Object execute()override { 
			*ptr = end_pos;
			return nullptr;
		}
	private:
		size_t* ptr;
		size_t end_pos;
	};
	namespace Parser
	{
		ParserNode* switch_statement();
	} 
}