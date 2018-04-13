#include "../include/value.hpp"
#include "../include/memory.hpp"
#include "../include/word_record.hpp"
Mer::Variable::Variable(Token * tok)
{
	auto result = global_symbol_table.find(Id::get_value(tok));
	if (result == nullptr)
		throw Error("var "+tok->to_string() + " no found");
	if (result->es != ESymbol::SVAR)
		throw Error(tok->to_string() + " must be a symbol");
	pos = static_cast<VarIdRecorder*> (result)->pos;
}

Mer::Mem::Object Mer::Variable::execute()
{
	return stack_memory[pos];
}
