#include "..\include\structure.hpp"

bool Mer::compareTypeInfo(const TypeInfo & a, const TypeInfo & b)
{
	return a.get_key() < b.get_key();
}

Mer::SComVarFuncCall::SComVarFuncCall(const std::vector<size_t>& types, FunctionBase * fun, std::vector<Expr*>& exprs):{}
