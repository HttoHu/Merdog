#include "../include/optimizer.hpp"
#include "../include/expr.hpp"
#include "../include/value.hpp"
namespace Mer
{
	namespace optimizer
	{
		ParserNode* optimize_array_subscript(ParserNode* arr, ParserNode* subscript)
		{
			type_code_index type=arr->get_type();

			if (typeid(*arr) == typeid(Variable)&& static_cast<Variable*>(arr)->arr())
			{
				int index = 0;
				if (typeid(*subscript) == typeid(LConV))
				{
					auto v = static_cast<LConV*>(subscript);
					// skip arr info
					index = Mem::get_raw<int>(v->execute())+1;
					// get element type;

					auto ret = new Variable(type, arr->get_pos() + index);
					delete arr;
					delete subscript;
					return ret;
				}
				else
				{
					auto ret= new ContainerIndex(type, arr->get_pos() + 1, subscript);
					delete arr;
					return ret;
				}
			}
			return new SubScript(arr, subscript);
		}

	}
}