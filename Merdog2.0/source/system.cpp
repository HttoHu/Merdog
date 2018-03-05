#include "..\include\syslib\system.hpp"
//#include "../include/type.hpp"
void Mer::Sys::import_io()
{
	import_function(-1,console_input,"cin");
	import_function(-1,console_output, "cout");
}

void Mer::Sys::import_function(size_t type,const std::function<Mem::Object(std::vector<Mem::Object>&)>& func, std::string name)
{
	auto tmp_pos = function_list.size();
	function_map.insert({ name,tmp_pos });
	function_list.push_back(new SystemFunction(type,func));
}

Mer::Mem::Object Mer::Sys::console_output(std::vector<Mer::Mem::Object>& args)
{
	for (auto &a:args)
	{
		if(a!=nullptr&&(a->get_type_code()==Mem::INT
			||a->get_type_code()==Mem::DOUBLE||a->get_type_code()==Mem::BOOL
			||a->get_type_code()==Mem::STRING))
			std::cout << a->to_string();
		else
		{
			throw Error("param error");
		}
	}
	return nullptr;
}

Mer::Mem::Object Mer::Sys::console_input(std::vector<Mer::Mem::Object>& args)
{
	if (args.size() != 1)
		throw Error("cin args not matched with param");
	auto tmp = args[0];
	switch (tmp->get_type_code())
	{
	case Mem::INT:
	{
		int64_t a = 0;
		std::cin >> a;
		std::static_pointer_cast<Mem::Int>(tmp)->reset_value(a);
		break;
	}
	case Mem::DOUBLE:
	{
		double a = 0;
		std::cin >> a;
		std::static_pointer_cast<Mem::Double>(tmp)->reset_value(a);
		break;
	}
	case Mem::STRING:
	{
		std::string a = 0;
		std::cin >> a;
		std::static_pointer_cast<Mem::String>(tmp)->reset_value(a);
		break;
	}
	}
	return tmp;
}
