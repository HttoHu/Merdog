#pragma once
#include <string>
#include <memory>
#include <deque>
#include <map>
#include "./defs.hpp"
#include "./parser_node.hpp"
namespace Mer {
	namespace Symbol {
	

		enum WordType {
			Variable, ConstVal, Function,Const
		};
		
		class WordRecorder {
		public:
			WordRecorder(WordType _word_type) : word_type(_word_type) {}
			virtual ~WordRecorder() {}
			virtual type_code_index get_type()const  { return 0; }
			virtual size_t get_pos() { return -1; }
			virtual std::string to_string() { return "unknown word"; }
			WordType get_word_type() { return word_type; }
		private:
			WordType word_type;
		};
		class VarRecorder:public WordRecorder {
		public:
			VarRecorder(bool _is_glo, size_t _pos, type_code_index _type_no) :WordRecorder(Variable),
				is_global(_is_glo),pos(_pos), type(_type_no) {}
			size_t get_pos()override { return pos; }
			type_code_index get_type()const override { return type; }
			bool is_global;
		private:
			size_t pos;
			type_code_index type;
		};
		// record const symbol,eg const int N=200005;
		class ConstRecorder :public WordRecorder {
		public:
			ConstRecorder(type_code_index _type_no,ParserNode* _node):WordRecorder(Const),type(_type_no),node(_node){ }
			type_code_index get_type()const override { return type; }
			static ParserNode* get_node(WordRecorder* wr) { return static_cast<ConstRecorder*>(wr)->node; }
		private:
			type_code_index type;
			ParserNode* node;
		};
		class SymbleTable {
		public:
			SymbleTable() { sym_tab.resize(1);}
			void new_block();
			void end_block();

			void push_glo_word(const std::string& name, WordRecorder* sym);
			void push_word(const std::string& name, WordRecorder* sym);
			WordRecorder* find(const std::string& name);

		private:
			std::deque<std::map<std::string, WordRecorder*>> sym_tab;
		};
	}
}