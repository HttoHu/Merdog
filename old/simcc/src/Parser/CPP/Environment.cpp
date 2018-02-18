#include "../HPP/Environment.hpp"
using namespace Simcc::Parser;
size_t Environment::current_pos;
Simcc::Runtime::Stack Environment::stack_block;
TokenStream* Environment::token_stream;