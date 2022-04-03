#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>

namespace Mer {
	using type_code_index = int;
	using type_len_t = int;

	class Type;
	// to record type info

	extern std::vector<Type*> type_tab;

	enum class TypeKind :int {
		BasicType, FuncType, ClassType, RefType
	};
	enum class BasicTypeTag :int {
		VOID = 0, INT, REAL, BYTE
	};

	size_t get_type_size(type_code_index ty);

	bool is_basic_type(type_code_index ty);

	type_code_index set_type(Type* ty);

	class Type {
	public:
		Type(const std::string& _name) :name(_name) {}
		std::string get_name() { return name; }
		virtual bool convertible(type_code_index ty) { return false; }
		virtual size_t type_length() { return 0; }
		virtual ~Type() {}
	private:
		TypeKind kind;
		std::string name;
	};

	class BasicType :public Type {
	public:
		BasicType(const std::string& _name, type_len_t _len, const std::set<type_code_index>& tab) :Type(_name), len(_len), conv_tab(tab) {}
		bool convertible(type_code_index ty)override { return conv_tab.count(ty); }
		size_t type_length()override { return len; }

	private:
		// 
		std::set<type_code_index> conv_tab;
		type_len_t len;
	};
	namespace Parser {
		type_code_index parse_type();
	}
}