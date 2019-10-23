#pragma once
#include "parser_node.hpp"
namespace mer
{
	class IntV final :public ParserNode
	{
	public:
		IntV(int v);
		IntV();
		std::string to_string()override;
	private:
		int value;
	};
	using IntVNode = std::shared_ptr<IntV>;
	class Variable :public ParserNode
	{
	public:
		Variable(std::string n);
		std::string to_string() override {
			return name;
		}
		virtual ~Variable() {}
	private:
		std::string name;
	};
}