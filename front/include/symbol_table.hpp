#pragma once
#include <string>
#include <deque>
#include <map>
#include "lexer.hpp"
namespace mer
{
	class Type;
	// the first argument is to reset index, if clear is true, and the index will be set to 0
	extern std::string get_tmp_var_name(bool clear = false);
	// to distinct the symbol's function.

	// parent class
	class WordRecorder
	{
	public:
		enum class SymbolTag :uint8_t
		{
			// defined in block, or tmp var
			LOCAL_VAR,
			GLO_VAR,
			FUNCTION_ID,
		};
		WordRecorder(const std::string& _name, Type* t, SymbolTag st) :name(_name), type(t), symbol_type(st) {}
		std::string get_name() { return name; }
		Type* type;
		SymbolTag symbol_type;
	private:

		std::string name;
	};
	class IdRecorder :public WordRecorder
	{
	public:
		IdRecorder(const std::string& _name, Type* t) :WordRecorder(_name, t, SymbolTag::LOCAL_VAR) {}
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
		void type_check(Token* id, WordRecorder::SymbolTag e);
		WordRecorder* find_front(std::string id);
		WordRecorder* find(std::string id);
		void push_glo(std::string id, WordRecorder* wr)
		{

			data.back().insert({ id,wr });
		}
		void push( WordRecorder* wr)
		{
			if (data.front().find(wr->get_name()) != data.front().end())
				throw Error("symbol redefined!");
			data.front().insert({wr->get_name(),wr });
		}
		void print();
	private:
		std::deque<std::map<std::string, WordRecorder*>> data;
	};
	// symbol table for user
	extern SymbolTable _symbol_table;
	WordRecorder* find_recorder_by_id(const std::string& str);
}