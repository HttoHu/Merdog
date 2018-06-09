#pragma once
#include "value.hpp"
namespace Mer
{
	enum ESymbol
	{
		SFUN,SVAR,SNAME,
		SGVAR,SSTRUCTURE
	};
	struct WordRecorder
	{
	public:
		WordRecorder(ESymbol e) :es(e) {}
		ESymbol es;
		size_t get_type() { return type_code; }
		virtual ~WordRecorder() {}
	protected:
		size_t type_code;
	};
	struct VarIdRecorder :public WordRecorder
	{
	public:
		VarIdRecorder(size_t type,size_t p,bool is_c=false) 
			:WordRecorder(SVAR),pos(p),is_const(is_c)
		{
			type_code = type;
		}
		size_t pos;
		bool known()
		{
			return is_const;
		}
		Mem::Object value()
		{
			if (is_const)
			{
				return obj;
			}
			throw Error("the var don't have an unknown value before execute.");
		}
	private:
		bool is_const;
		Mem::Object obj;
	};
	struct StructRecorder :public WordRecorder
	{
	public:
		StructRecorder() :WordRecorder(ESymbol::SSTRUCTURE)
		{
			type_code = Mem::type_no()++;
		}
		
	private:
	};
	class Namespace;

	struct GVarIdRecorder:public WordRecorder
	{
	public:
		GVarIdRecorder(size_t t, Mem::Object obj) :WordRecorder(SGVAR), value(obj) {
			type_code = t;
		}
		Mem::Object value;
	};
	struct FuncIdRecorder :public WordRecorder
	{
	public:
		FuncIdRecorder(size_t type) :WordRecorder(ESymbol::SFUN){
			type_code = type;
		}
	private:
	};
	class SymbolTable
	{
	public:
		SymbolTable()
		{
			data.push_front(std::map<std::string, WordRecorder*>());
		}
		void new_block()
		{
			data.push_front(std::map<std::string, WordRecorder*>());
		}
		void end_block()
		{
			data.pop_front();
		}
		void type_check(Token *id, ESymbol e)
		{
			auto result = find(Id::get_value(id));
			if (result == nullptr)
				throw Error("id " + id->to_string() + " no found.");
			if (result->es != e)
				throw Error("id typ not matched");
		}
		WordRecorder* find_front(std::string id)
		{
			auto result = data.front().find(id);
			if (result == data.front().end())
				return nullptr;
			return result->second;
		}
		WordRecorder* find(std::string id)
		{
			for (const auto &a:data)
			{
				auto result = a.find(id);
				if (result != a.end())
				{
					return result->second;
				}
			}
			return nullptr;
		}
		void push_glo(std::string id, WordRecorder *wr)
		{
			data.back().insert({ id,wr });
		}
		void push(std::string id, WordRecorder *wr)
		{
			data.front().insert({ id,wr });
		}
	private:
		std::deque<std::map<std::string, WordRecorder*>> data;
	};
}