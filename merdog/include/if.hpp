#pragma once
#include "parser.hpp"
namespace Mer
{
	class If :public ParserNode
	{
	public:
		Mem::Object execute()override;
		std::vector<std::pair<Expr*, Block*>> if_block;
		Block* else_block;
	};
	namespace Parser
	{
		ParserNode *if_statement();
	}
}