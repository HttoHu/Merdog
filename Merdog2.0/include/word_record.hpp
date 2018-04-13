#pragma once
#include "value.hpp"
namespace Mer
{
	enum ESymbol
	{
		SFUN,SVAR,SNAME,
	};
	struct WordRecorder
	{
	public:
		WordRecorder(ESymbol e) :es(e) {}
		ESymbol es;
		int word_type;
		virtual ~WordRecorder() {}
	};
	struct VarIdRecorder :public WordRecorder
	{
	public:
		VarIdRecorder( int type,size_t p,bool is_c=false) 
			:WordRecorder(SVAR),pos(p),is_const(is_c){}
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
		void push(std::string id, WordRecorder *wr)
		{
			data.front().insert({ id,wr });
		}
	private:
		std::deque<std::map<std::string, WordRecorder*>> data;
	};
	extern SymbolTable global_symbol_table;
}