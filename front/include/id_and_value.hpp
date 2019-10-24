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
	class RealV final :public ParserNode
	{
	public:
		RealV(double v);
		RealV();
		std::string to_string()override;
	private:
		double value;
	};
	class BoolV final :public ParserNode
	{
	public:
		BoolV(bool v);
		BoolV();
		std::string to_string()override
		{
			if (value)
				return "true";
			return "false";
		}
	private:
		bool value;
	};
	class CharV final :public ParserNode
	{
	public:
		CharV(char v);
		CharV();
		std::string to_string()override
		{
			return "c\'"+std::string(1, value)+"\'";
		}
	private:
		char value;
	};
	using RealVNode = std::shared_ptr<RealV>;
	using IntVNode = std::shared_ptr<IntV>;
	using CharVNode = std::shared_ptr<CharV>;
	using BoolVNode = std::shared_ptr<BoolV>;
	class Variable :public ParserNode
	{
	public:
		Variable(std::string n);
		std::string to_string() override {
			return "%"+name;
		}
		virtual ~Variable() {}
	private:
		std::string name;
	};
}