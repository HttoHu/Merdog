#pragma once
#include "lexer.hpp"
#include <memory>
namespace mer
{
	struct instruction_param;
	using instruction = void(*)(const instruction_param & p);
	enum class NodeTag:int {
		CINTV
	};
	class ParserNode
	{
	public:
		ParserNode(NodeTag nt) : node_tag(nt) {}
		virtual ~ParserNode() {}
		virtual std::string get_gen();
		virtual std::string to_string();
		virtual instruction get_instruction();
		virtual size_t get_type();
		NodeTag get_tag() { return node_tag; }
	protected:
		NodeTag node_tag;
	};
	using Node = std::shared_ptr<ParserNode>;
}