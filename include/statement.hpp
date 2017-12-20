#pragma once
#include "action.hpp"
namespace Merdog
{
	class Statement
	{
	public:
		enum StatementType
		{
			_ExprUnit,_If,_CreateVar,_For,_While,
			_Switch,
		};
		Statement(StatementType v) :st(v) {}
		virtual ~Statement() = 0;

		virtual void run() = 0;
		virtual std::string to_string()const = 0;
	private:
		StatementType st;
	};
	class ExprUnit:public Statement
	{
	public:
		enum ExprUnitType {
			Single,ThreeAdd,TwoAdd
		};
		ExprUnit(ExprUnitType e) :Statement(_ExprUnit),et(e) {}
	private:
		ExprUnitType et;
	};
	class Single final:public ExprUnit
	{
	public:

	private:
		//Action op;
	};
}