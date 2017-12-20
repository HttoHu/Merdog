#include "../include/object.hpp"
#include "../include/error.hpp"
#include <time.h>
using namespace Merdog;
// sure when we count, we should assure that the typeof val and out and this are same.
// or exceptable.
void Merdog::Object::Int::count(ObjectBase * val, OperType type, ObjectBase * out)
{
	switch (type)
	{
	case Merdog::Object::_Add:
		static_cast<Int*>(out)->value = value + static_cast<Int*>(val)->value;
		return;
	case Merdog::Object::_Sub:
		static_cast<Int*>(out)->value = value - static_cast<Int*>(val)->value;
		return;
	case Merdog::Object::_Mul:
		static_cast<Int*>(out)->value = value * static_cast<Int*>(val)->value;
		return;
		/*
		switch (val->obj_type)
		{
		case _Int:
			*out = Int(static_cast<Int*>(val)->value * value);
			return;
		case _Double:
			*out = Int(static_cast<Double*>(val)->value * value);
			return;
		case _Long:
			*out = Int(static_cast<Long*>(val)->value * value);
			return;
		case _Char:
			*out = Int(static_cast<Char*>(val)->value * value);
			return;

		default:
			throw Error("type not matched");
		}
		*/
	case Merdog::Object::_Div:
		static_cast<Int*>(out)->value = value / static_cast<Int*>(val)->value;
		return;
	case Merdog::Object::_Inc:
		value++;
		return;
	case Merdog::Object::_Dec:
		value--;
		return;
	case Merdog::Object::_Not:
		static_cast<Int*>(out)->value = -value;
		return;
	case Merdog::Object::_SAdd:
		value += static_cast<Int*>(val)->value;
		return;
	case Merdog::Object::_SSub:
		value -= static_cast<Int*>(val)->value;
		return;
	case Merdog::Object::_SMul:
		value *= static_cast<Int*>(val)->value;
		return;
	case Merdog::Object::_SDiv:
		value /= static_cast<Int*>(val)->value;
		return;
	case Merdog::Object::_Assign:
		value = static_cast<Int*>(val)->value;
		return;
	case Object::_Eq:
		static_cast<Bool*>(out)->value = value==static_cast<Int*>(val)->value;
		return;
	case Object::_Ne:
		static_cast<Bool*>(out)->value = value != static_cast<Int*>(val)->value;
		return;
	case Object::_Ge:
		static_cast<Bool*>(out)->value = value >= static_cast<Int*>(val)->value;
		return;
	case Object::_Le:
		static_cast<Bool*>(out)->value = value <= static_cast<Int*>(val)->value;
		return;
	case Object::_Gt:
		static_cast<Bool*>(out)->value = value > static_cast<Int*>(val)->value;
		return;
	case Object::_Lt:
		static_cast<Bool*>(out)->value = value < static_cast<Int*>(val)->value;
		return;
	default:
		break;
	}
}

void Merdog::Object::Bool::count(ObjectBase * val, OperType type, ObjectBase * out)
{
	switch (type)
	{
	case Merdog::Object::_Assign:
		value=static_cast<Bool*>(val)->value;
		return;
	case _Not:
		static_cast<Bool*>(out)->value = !value;
		return;
	default:
		break;
	}
}
/*
int test_object()
{
	using namespace Merdog;
	try
	{
		time_t start = clock();
		std::vector<Object::ObjectBase*> stack;
		size_t pos = 0;
		stack.push_back(new Object::Int(0));
		Object::ObjectBase *sum = stack[0];
		Object::ObjectBase* i = new Object::Int(0);
		Object::ObjectBase *tmp = new Object::Bool(true);
		Object::ObjectBase *cond = new Object::Int(100000000);
		while (static_cast<Object::Bool*>(tmp)->value == true)
		{
			i->count(cond, Object::_Lt, tmp);
			i->count(nullptr, Object::_Inc, nullptr);
			stack[0]->count(i, Object::_SAdd, nullptr);
		}

		std::cout << "the value is: " << sum->to_string() << std::endl;
		std::cout << "time:" << (double)(end - start) / CLK_TCK;
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
	std::cin.get();
	return 0;
}
*/