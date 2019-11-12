#pragma once
#include <iostream>
#include "parser_node.hpp"

namespace Mer
{
	namespace optimizer
	{
		ParserNode* optimize_array_subscript(ParserNode* arr, ParserNode* subscript);
	}
}