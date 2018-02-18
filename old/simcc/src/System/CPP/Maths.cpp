#include "../HPP/Maths.hpp"

Simcc::Runtime::ObjectBase * Simcc::System::sqrt(Parser::Param * param)
{
	if (param->param_list.size() != 1)
		throw std::runtime_error("param not matched");
	auto obj = param->param_list[0];
	double d = 0.0;
	switch (obj->get_tag())
	{
	case Lexer::Id:
		d = Parser::Environment::stack_block.find_variable(obj)->to_double();
		break;
	case Lexer::TLiteralDouble:
		d = *(double*)obj->get_value();
		break;
	case Lexer::TLiteralInt:
		d = *(int*)obj->get_value();
		break;
	case Lexer::TLiteralLong:
		d = *(int64_t*)obj->get_value();
		break;
	case Lexer::TLiteralChar:
		d = *(char*)obj->get_value();
		break;
	default:
		throw std::runtime_error("damn it");
		break;
	}
	double r = std::sqrt(d);
	return new Simcc::Runtime::ObjectBase(r);
}
