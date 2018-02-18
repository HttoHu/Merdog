#include "../HPP/Object.hpp"
using namespace Simcc::Runtime;
int32_t Simcc::Runtime::ObjectBase::to_int()const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return *(int32_t*)data;
	case Simcc::Runtime::ObjectBase::Long:
		return (int32_t)*(long*)data;
	case Simcc::Runtime::ObjectBase::Double:
		return (int32_t)*(double*)data;
	case Simcc::Runtime::ObjectBase::Char:
		return (int32_t)*(char*)data;
	case Simcc::Runtime::ObjectBase::String:
		throw std::runtime_error("double Simcc::Runtime::ObjectBase::get_float_value()");
	case Simcc::Runtime::ObjectBase::Bool:
		return (int32_t)*(bool*)data;
	default:
		throw std::runtime_error("double Simcc::Runtime::ObjectBase::get_float_value() ");
	}
}
double Simcc::Runtime::ObjectBase::to_double()const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return (double)*(int32_t*)data;
	case Simcc::Runtime::ObjectBase::Long:
		return (double)*(long*)data;
	case Simcc::Runtime::ObjectBase::Double:
		return *(double*)data;
	case Simcc::Runtime::ObjectBase::Char:
		return (double)*(char*)data;
	case Simcc::Runtime::ObjectBase::String:
		throw std::runtime_error("double Simcc::Runtime::ObjectBase::get_float_value()");
	case Simcc::Runtime::ObjectBase::Bool:
		return (double)*(bool*)data;
	default:
		throw std::runtime_error("double Simcc::Runtime::ObjectBase::get_float_value() ");
	}
}
int64_t Simcc::Runtime::ObjectBase::to_long()const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return (int64_t)*(int32_t*)data;
	case Simcc::Runtime::ObjectBase::Long:
		return (int64_t)*(long*)data;
	case Simcc::Runtime::ObjectBase::Double:
		return (int64_t)*(double*)data;
	case Simcc::Runtime::ObjectBase::Char:
		return (int64_t)*(char*)data;
	case Simcc::Runtime::ObjectBase::String:
		throw std::runtime_error("double Simcc::Runtime::ObjectBase::get_float_value()");
	case Simcc::Runtime::ObjectBase::Bool:
		return (int64_t)*(bool*)data;
	default:
		throw std::runtime_error("double Simcc::Runtime::ObjectBase::get_float_value() ");
	}
}

bool Simcc::Runtime::ObjectBase::to_bool() const
{
	return *(bool*)data;
}

std::string Simcc::Runtime::ObjectBase::to_string()const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return std::to_string(*(int32_t*)data);
	case Simcc::Runtime::ObjectBase::Long:
		return std::to_string(*(long*)data);
	case Simcc::Runtime::ObjectBase::Double:
		return std::to_string(*(double*)data);
	case Simcc::Runtime::ObjectBase::Char:
		return std::to_string((int64_t)*(char*)data);
	case Simcc::Runtime::ObjectBase::String:
		return *(std::string*)data;
	case Simcc::Runtime::ObjectBase::Bool:
		if (*(bool*)data)
		{
			return "true";
		}
		return "false";
	default:
		throw std::runtime_error("double Simcc::Runtime::ObjectBase::to_string() ");
	}
}

ObjectBase * Simcc::Runtime::ObjectBase::Add(const ObjectBase * obj1)
{
	ObjectBase *ret = new ObjectBase(*this+*obj1);
	return ret;
}
ObjectBase * Simcc::Runtime::ObjectBase::Sub(const ObjectBase * obj1)
{
	ObjectBase *ret = new ObjectBase(*this-*obj1 );
	return ret;
}
ObjectBase * Simcc::Runtime::ObjectBase::Mul(const ObjectBase * obj1)
{
	ObjectBase *ret = new ObjectBase(*this * *obj1);
	return ret;
}
ObjectBase * Simcc::Runtime::ObjectBase::Div(const ObjectBase * obj1)
{
	ObjectBase *ret = new ObjectBase(*this / *obj1);
	return ret;
}
ObjectBase* Simcc::Runtime::ObjectBase::Compare(const ObjectBase * obj, Simcc::Lexer::Tag t)
{
	switch (t)
	{
	case Simcc::Lexer::Ge:
		return new ObjectBase(*this >= *obj);
	case Simcc::Lexer::Gt:
		return new ObjectBase(*this > *obj);
	case Simcc::Lexer::Le:
		return new ObjectBase(*this <= *obj);
	case Simcc::Lexer::Lt:
		return new ObjectBase(*this < *obj);
	case Simcc::Lexer::Ne:
		return new ObjectBase(*this != *obj);
	case Simcc::Lexer::Eq:
		return new ObjectBase(*this == *obj);
	case Simcc::Lexer::And:
		return new ObjectBase(get_value<bool>() &&obj->get_value<bool>());
	case Simcc::Lexer::Or:
		return new ObjectBase(get_value<bool>() || obj->get_value<bool>());
	default:
		break;
	}


}
ObjectBase Simcc::Runtime::ObjectBase::operator+(const ObjectBase & v) const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return ObjectBase(*(int32_t*)data + v.to_int());
	case Simcc::Runtime::ObjectBase::Long:
		return ObjectBase(*(int64_t*)data + v.to_long());
	case Simcc::Runtime::ObjectBase::Double:
		return ObjectBase(*(double*)data + v.to_double());
	case Simcc::Runtime::ObjectBase::Char:
		return ObjectBase(*(char*)data + *(char*)v.data);
	case Simcc::Runtime::ObjectBase::String:
		return ObjectBase(*(std::string*)data + v.to_string());
	default:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator+(const ObjectBase & v) const ");
	}
}
ObjectBase Simcc::Runtime::ObjectBase::operator-(const ObjectBase & v) const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return ObjectBase(*(int32_t*)data - v.to_int());
	case Simcc::Runtime::ObjectBase::Long:
		return ObjectBase(*(int64_t*)data - v.to_long());
	case Simcc::Runtime::ObjectBase::Double:
		return ObjectBase(*(double*)data - v.to_double());
	case Simcc::Runtime::ObjectBase::Char:
		return ObjectBase(*(char*)data - *(char*)v.data);
	case Simcc::Runtime::ObjectBase::String:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator-(const ObjectBase & v) const ");
	default:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator-(const ObjectBase & v) const ");
	}
}

ObjectBase Simcc::Runtime::ObjectBase::operator*(const ObjectBase & v) const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return ObjectBase(*(int32_t*)data * v.to_int());
	case Simcc::Runtime::ObjectBase::Long:
		return ObjectBase(*(int64_t*)data * v.to_long());
	case Simcc::Runtime::ObjectBase::Double:
		return ObjectBase(*(double*)data * v.to_double());
	case Simcc::Runtime::ObjectBase::Char:
		return ObjectBase(*(char*)data * *(char*)v.data);
	case Simcc::Runtime::ObjectBase::String:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator*(const ObjectBase & v) const ");
	default:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator*(const ObjectBase & v) const ");
	}
}
ObjectBase Simcc::Runtime::ObjectBase::operator/(const ObjectBase & v) const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return ObjectBase(*(int32_t*)data / v.to_int());
	case Simcc::Runtime::ObjectBase::Long:
		return ObjectBase(*(int64_t*)data / v.to_long());
	case Simcc::Runtime::ObjectBase::Double:
		return ObjectBase(*(double*)data / v.to_double());
	case Simcc::Runtime::ObjectBase::Char:
		return ObjectBase(*(char*)data / *(char*)v.data);
	case Simcc::Runtime::ObjectBase::String:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator/(const ObjectBase & v) const ");
	default:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator/(const ObjectBase & v) const ");
	}
}
ObjectBase Simcc::Runtime::ObjectBase::operator==(const ObjectBase & v) const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return ObjectBase(*(int32_t*)data == v.to_int());
	case Simcc::Runtime::ObjectBase::Long:
		return ObjectBase(*(int64_t*)data == v.to_long());
	case Simcc::Runtime::ObjectBase::Double:
		return ObjectBase(*(double*)data == v.to_double());
	case Simcc::Runtime::ObjectBase::Char:
		return ObjectBase(*(char*)data == *(char*)v.data);
	case Simcc::Runtime::ObjectBase::String:
		return ObjectBase(*(std::string*)data == v.to_string());
	default:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator==(const ObjectBase & v) const ");
	}
}
ObjectBase Simcc::Runtime::ObjectBase::operator>=(const ObjectBase & v) const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return ObjectBase(*(int32_t*)data >= v.to_int());
	case Simcc::Runtime::ObjectBase::Long:
		return ObjectBase(*(int64_t*)data >= v.to_long());
	case Simcc::Runtime::ObjectBase::Double:
		return ObjectBase(*(double*)data >= v.to_double());
	case Simcc::Runtime::ObjectBase::Char:
		return ObjectBase(*(char*)data >= *(char*)v.data);
	case Simcc::Runtime::ObjectBase::String:
		return ObjectBase(*(std::string*)data >= v.to_string());
	default:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator>=(const ObjectBase & v) const ");
	}
}
ObjectBase Simcc::Runtime::ObjectBase::operator<=(const ObjectBase & v) const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return ObjectBase(*(int32_t*)data <= v.to_int());
	case Simcc::Runtime::ObjectBase::Long:
		return ObjectBase(*(int64_t*)data <= v.to_long());
	case Simcc::Runtime::ObjectBase::Double:
		return ObjectBase(*(double*)data <= v.to_double());
	case Simcc::Runtime::ObjectBase::Char:
		return ObjectBase(*(char*)data <= *(char*)v.data);
	case Simcc::Runtime::ObjectBase::String:
		return ObjectBase(*(std::string*)data <= v.to_string());
	default:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator<=(const ObjectBase & v) const ");
	}
}
ObjectBase Simcc::Runtime::ObjectBase::operator!=(const ObjectBase & v) const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return ObjectBase(*(int32_t*)data != v.to_int());
	case Simcc::Runtime::ObjectBase::Long:
		return ObjectBase(*(int64_t*)data != v.to_long());
	case Simcc::Runtime::ObjectBase::Double:
		return ObjectBase(*(double*)data != v.to_double());
	case Simcc::Runtime::ObjectBase::Char:
		return ObjectBase(*(char*)data != *(char*)v.data);
	case Simcc::Runtime::ObjectBase::String:
		return ObjectBase(*(std::string*)data != v.to_string());
	default:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator!=(const ObjectBase & v) const ");
	}
}
ObjectBase Simcc::Runtime::ObjectBase::operator>(const ObjectBase & v) const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return ObjectBase(*(int32_t*)data > v.to_int());
	case Simcc::Runtime::ObjectBase::Long:
		return ObjectBase(*(int64_t*)data > v.to_long());
	case Simcc::Runtime::ObjectBase::Double:
		return ObjectBase(*(double*)data > v.to_double());
	case Simcc::Runtime::ObjectBase::Char:
		return ObjectBase(*(char*)data > *(char*)v.data);
	case Simcc::Runtime::ObjectBase::String:
		return ObjectBase(*(std::string*)data > v.to_string());
	default:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator>(const ObjectBase & v) const ");
	}
}
ObjectBase Simcc::Runtime::ObjectBase::operator<(const ObjectBase & v) const
{
	switch (type)
	{
	case Simcc::Runtime::ObjectBase::Int:
		return ObjectBase(*(int32_t*)data < v.to_int());
	case Simcc::Runtime::ObjectBase::Long:
		return ObjectBase(*(int64_t*)data < v.to_long());
	case Simcc::Runtime::ObjectBase::Double:
		return ObjectBase(*(double*)data < v.to_double());
	case Simcc::Runtime::ObjectBase::Char:
		return ObjectBase(*(char*)data < *(char*)v.data);
	case Simcc::Runtime::ObjectBase::String:
		return ObjectBase(*(std::string*)data < v.to_string());
	default:
		throw std::runtime_error("ObjectBase Simcc::Runtime::ObjectBase::operator<(const ObjectBase & v) const ");
	}
}