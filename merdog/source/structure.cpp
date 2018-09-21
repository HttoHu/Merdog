#include "..\include\structure.hpp"

bool Mer::compareTypeInfo(const TypeInfo & a, const TypeInfo & b)
{
	return a.get_key() < b.get_key();
}