#pragma once
#include "lexer.hpp"
#include <memory>
namespace mer
{
	class Type;
	class ParserNode
	{
	public:
		// to identify the node's type.
		enum NodeTag {
			CINTV=0,
			BINOP,
			NONOP,
			LOCAL_VARDECl,
			VARIABLE,
			REAL_VALUE,
			BOOL_VALUE,
			CHAR_VALUE
		};
		ParserNode(NodeTag nt) : node_tag(nt) {}
		virtual ~ParserNode() {}
		virtual void emit_gen();
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
	using Node = std::shared_ptr<ParserNode>;
	std::string type_convert(Type* s, Type* dest, Node n);
	template<typename _Ty>
	_Ty void_cast(void* p)
	{
		return *(_Ty*)(p);
	}

}