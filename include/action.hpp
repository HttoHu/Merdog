#pragma once
#include <iostream>
#include "lexer.hpp"
// action is expr's unit.
namespace Merdog
{
	class ActionBase
	{
	public:
		enum ActionType {
			Lit,Single, Call, Index, Inc, Dec, Member,Operator,
		};
		ActionBase(Token *v) :value(v) 
		{
			switch (v->get_token_type())
			{
			case TId:
				action_type = Single;
				return;
			default:
				action_type = Operator;
				return;
			}
		}
		ActionBase(Token *v, ActionType ac) :value(v), action_type(ac) {}
		virtual std::string to_string()const
		{
			return std::to_string(*value);
		}
		virtual ~ActionBase() {}
		ActionType get_type() {
			return action_type;
		}
		Id *get_id()
		{
			return static_cast<Id*>(value);
		}
	protected:
		Token *value;
		ActionType action_type;
	};
	class ActionCall :public ActionBase
	{
	public:
	private:

	};
	template<bool front=true>
	class ActionInc :public ActionBase
	{
	public:
		ActionInc(Id *id) :ActionBase(id, Inc),is_front(front) {}
		std::string to_string()const override
		{
			return ActionBase::to_string()+"++";
		}
	private:
		bool is_front;
	};
	template<bool front=true>
	class ActionDec :public ActionBase
	{
	public:
		ActionDec(Id *id) :ActionBase(id, Dec),is_front(front) {}
		std::string to_string()const override
		{
			return ActionBase::to_string() + "--";
		}
	private:
		bool is_front;
	};
	class ActionLiteral :public ActionBase
	{
	public:
		ActionLiteral(Token *lt) :ActionBase(lt, Lit) {}
		Literal* get_value()
		{
			return static_cast<Literal*>(value);
		}
		std::string to_string()
		{
			return ActionBase::to_string();
		}
	private:

	};
}