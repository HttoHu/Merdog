#pragma once
#include <string>
#include "parser_node.hpp"
namespace mer
{
	extern std::vector<std::string> ir_instructions;
	class Type;
	// NamePart parse the var_name and its attached property for instance , a[3] , a[3] is an array and be packed to name_part
	class NamePart
	{
	public:
		NamePart();
		std::string get_name() {
			return name;
		}
	private:
		std::string name;
	};
	// in a var_decl , you may def many var in one sentance for instance
	// int a=3,b=4,c=3;
	class VarDeclUnit
	{
	public:
		VarDeclUnit();
		Type* get_type() {
			return type;
		}
		NamePart& get_name_part()
		{
			return name_part;
		}
		Node expr;
	private:
		NamePart name_part;
		Type* type;

	};
	using VarDeclUnitPtr = std::shared_ptr<VarDeclUnit>;
	class LocalVarDecl :public ParserNode
	{
	public:
		LocalVarDecl(std::vector<VarDeclUnitPtr>& vec, Type *t);
		void emit_gen()override;
	private:
		std::vector<VarDeclUnitPtr> units;
	};
	using LocalVarDeclPtr = std::shared_ptr< LocalVarDecl>;
	namespace parse_statement
	{
		Node var_decl();
		Node statement();
	}
}