#pragma once
#include "lexer.hpp"
#include <memory>
namespace mer
{
	class Type;
	enum class NodeTag:int {
		CINTV
	};
	class ParserNode
	{
	public:
		enum NodeTag {
			CINTV=0,
			BINOP,
		};
		ParserNode(NodeTag nt) : node_tag(nt) {}
		virtual ~ParserNode() {}
		virtual std::string get_gen();
		//get tmp var name;
		virtual std::string to_string();
		virtual Type* get_type();
		virtual void* get_data() { return nullptr; }
		void set_type(Type* t)
		{
			type = t;
		}
		NodeTag get_tag() { return node_tag; }
	protected:
		Type* type;
		NodeTag node_tag;
	};
	template<typename _Ty>
	_Ty void_cast(void* p)
	{
		return *(_Ty*)(p);
	}
	using Node = std::shared_ptr<ParserNode>;
}