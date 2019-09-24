#pragma once
#include "value.hpp"
#include <functional>
namespace Mer
{
	enum ESymbol
	{
		SFUN,SVAR,SNAME,
		SGVAR,SSTRUCTURE,
		STYPE,USVAR, SPOINTER,
	};
	struct WordRecorder
	{
	public:
		WordRecorder(ESymbol e, size_t tc = 0) :es(e),type_code(tc) {}
		ESymbol es;
		virtual size_t get_pos() { return 0; }
		virtual std::string to_string() { return""; }
		size_t get_type() { return type_code; }
		virtual ~WordRecorder() {}
	protected:
		size_t type_code;
	};
	struct VarIdRecorder :public WordRecorder
	{
	public:
		VarIdRecorder(size_t type,size_t p,bool is_c=false,WordRecorder wr=SVAR) 
			:WordRecorder(wr),pos(p),is_const(is_c)
		{
			type_code = type;
		}
		size_t pos;
		size_t get_pos()override {
			return pos + mem.get_current();
		}
		std::string to_string()override
		{
			return "pos:" + std::to_string(pos);
		}
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
			type_code = size_t(Mem::type_counter+=2);
		}

	};
	class Namespace;
	struct GVarIdRecorder:public WordRecorder
	{
	public:
		GVarIdRecorder(size_t t,size_t _pos) :WordRecorder(SGVAR,t), pos(_pos) {
		}
		size_t get_pos()override
		{
			return pos;
		}
		Mem::Object& get_value()
		{
			return mem[pos];
		}
		size_t pos;
	};
	struct FuncIdRecorder :public WordRecorder
	{
	public:
		FuncIdRecorder(size_t type) :WordRecorder(ESymbol::SFUN){
			type_code = type;
		}
	private:
	};
	struct BuildInClass :public WordRecorder
	{
	public:
		BuildInClass(const std::string &str):WordRecorder(STYPE)
		{
			type_code = Mem::type_counter += 2;
		}
		std::string to_string() override { 
			return "type:"+type_name; 
		}
		std::function<ParserNode*()> create_var;
		std::function<ParserNode*()> create_glo_var;
	private:
		std::string type_name;
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
			for (int i=0;i<data.size();i++)
			{
				auto result = data[i].find(id);
				if (result != data[i].end())
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
			//std::cout << id << "   " << wr->to_string() << std::endl;
		}
		void print();
	private:
		std::deque<std::map<std::string, WordRecorder*>> data;
	};
}