#include "../include/optimizer.hpp"
#include "../include/expr.hpp"
#include "../include/value.hpp"
namespace Mer
{
	namespace optimizer
	{
		ParserNode* optimize_array_subscript(ParserNode* arr, ParserNode* subscript)
		{

			if (typeid(*arr) == typeid(Variable)&& static_cast<Variable*>(arr)->arr())
			{
				int index = 0;
				if (typeid(*subscript) == typeid(LConV))
				{
					auto v = static_cast<LConV*>(subscript);
					// skip arr info
					index = Mem::get_raw<int>(v->execute())+1;
					return new Variable(arr->get_type(), arr->get_pos() + index);
				}
				else
				{
					return new ContainerIndex(arr->get_type(), arr->get_pos(), subscript);
				}
			}
			return new SubScript(arr, subscript);
		}

	}
}