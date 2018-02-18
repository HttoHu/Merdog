#include "..\HPP\ConsoleIO.hpp"

Simcc::Runtime::ObjectBase * Simcc::System::console_output(Parser::Param * param) {
	auto a = param->get_list();
	if (a.size() != 1)
		throw Error::SyntaxError("param not matched");
	std::cout << a.front()->to_string();
	return nullptr;
}

Simcc::Runtime::ObjectBase * Simcc::System::console_output_line(Parser::Param * param)
{
	auto a = param->get_list();
	if (a.size() != 1)
		throw Error::SyntaxError("param not matched");
	std::cout << a.front()->to_string()<<std::endl;
	return nullptr;
}

Simcc::Runtime::ObjectBase * Simcc::System::console_input(Parser::Param * param)
{
	if (param->param_list.size() != 1)
		throw Error::SyntaxError("param not matched");
	auto obj = Parser::Environment::stack_block.find_variable(param->param_list[0]);
	switch (obj->type)
	{
	case Simcc::Runtime::ObjectBase::Int:
	{
		int a;
		std::cin >> a;
		*obj = Simcc::Runtime::ObjectBase(a);
		break;
	}
	case Simcc::Runtime::ObjectBase::String:
	{
		std::string a;
		std::cin >> a;
		*obj = Simcc::Runtime::ObjectBase(a);
		break;
	}
	case Simcc::Runtime::ObjectBase::Char:
	{
		char a;
		std::cin >> a;
		*obj = Simcc::Runtime::ObjectBase(a);
		break;
	}
	case Simcc::Runtime::ObjectBase::Double:
	{
		double a;
		std::cin >> a;
		*obj = Simcc::Runtime::ObjectBase(a);
		break;
	}
	case Simcc::Runtime::ObjectBase::Long:
	{
		int64_t a;
		std::cin >> a;
		*obj = Simcc::Runtime::ObjectBase(a);
		break;
	}
	default:
		break;
	}
}