#pragma once
#include "lexer.hpp"
namespace Mer
{
	extern std::string output_buff;
	std::string run_interpreter(const std::string&file_content);
}