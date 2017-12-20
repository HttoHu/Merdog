#include "../HPP/IdTypeTable.hpp"
#include "../HPP/../../Lexer/HPP/Id.hpp"
using namespace Simcc;
std::unordered_map<Lexer::Token*, size_t> Runtime::IdTypeTable::type_table;

size_t Simcc::Runtime::IdTypeTable::find_type(Lexer::Token * tok)
{
	auto result = type_table.find(tok);
	if (result == type_table.end())
		throw std::runtime_error("no found type " + tok->to_string());
	return result->second;
}
