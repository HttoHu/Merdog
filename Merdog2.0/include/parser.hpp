#pragma once
#include "expr.hpp"
#include "block.hpp"
namespace Mer
{
	class Program:public ParserNode
	{
	public:
		Program(Token *id,Block* b):identify(id),blo(b) {}
		Mem::Object execute()override
		{
			blo->run();
		}
		std::string to_string()override
		{
			return identify->to_string();
		}
	private:
		Token *identify;
		Block *blo;
	};
	class VarDecl :public ParserNode
	{
	public:
		VarDecl(const std::map<Token*, Expr*> &v);
		Mem::Object execute()override;
	private:
		size_t type;
		std::vector<std::size_t, Expr *> var_list;
	};
	namespace Parser
	{
		Program* program();
		Block* block();
	}
}