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
		enum NodeTag {
			CINTV=0
		};
		ParserNode(NodeTag nt) : node_tag(nt) {}
		virtual ~ParserNode() {}
		virtual std::string get_gen();
		virtual std::string to_string();
		virtual instruction get_instruction();
		virtual size_t get_type();
		virtual void* get_data() { return nullptr; }
		NodeTag get_tag() { return node_tag; }
	protected:
		
		NodeTag node_tag;
	};
	template<typename _Ty>
	_Ty void_cast(void* p)
	{
		return *(_Ty*)(p);
	}
	using Node = std::shared_ptr<ParserNode>;
}