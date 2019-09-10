/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/clib/merdog_io.hpp"

namespace Mer
{
	namespace
	{
		Mem::Object _substr(std::vector<Mem::Object>& args)
		{
			if (args.size() == 3)
			{
				auto tmp = std::dynamic_pointer_cast<Mem::String>(args[0]);
				auto off= std::dynamic_pointer_cast<Mem::Int>(args[1]);
				auto size = std::dynamic_pointer_cast<Mem::Int>(args[2]);
				return std::make_shared<Mem::String>(tmp->to_string().substr(off->get_value(),size->get_value()));
			}
			throw Error("argument size error");
		}
		Mem::Object _str_size(std::vector<Mem::Object>& args)
		{
			if (args.size() == 1)
			{
				auto tmp = std::dynamic_pointer_cast<Mem::String>(args[0]);
				return std::make_shared<Mem::Int>(tmp->to_string().size());
			}
			throw Error("argument size error");
		}
		Mem::Object _cout(std::vector<Mem::Object>& args)
		{
			for (const auto &a : args)
				std::cout << a->to_string();
			return nullptr;
		}
		Mem::Object _input_int(std::vector<Mem::Object>& args)
		{
			if (args.size() != 0)
				throw Error("argument size error");
			int obj = 0;
			std::cin >> obj;
			return std::make_shared<Mem::Int>(obj);
		}
		Mem::Object _input_real(std::vector<Mem::Object>& args)
		{
			if (args.size() != 0)
				throw Error("argument size error");
			double obj = 0;
			std::cin >> obj;
			return std::make_shared<Mem::Double>(obj);
		}
		Mem::Object _input_string(std::vector<Mem::Object>& args)
		{
			if (args.size() != 0)
				throw Error("argument size error");
			std::string obj = 0;
			std::cin >> obj;
			return std::make_shared<Mem::String>(obj);
		}
	}
	Namespace *mstd=new Namespace(nullptr);
	Mer::SystemFunction *substr = new SystemFunction(Mem::BasicType::STRING, _substr);
	Mer::SystemFunction *str_size = new SystemFunction(Mem::BasicType::STRING, _str_size);
	Mer::SystemFunction *cout=new SystemFunction(Mem::BasicType::BVOID,_cout);
	Mer::SystemFunction *input_int=new SystemFunction(Mem::BasicType::INT, _input_int);
	Mer::SystemFunction *input_real = new SystemFunction(Mem::BasicType::DOUBLE, _input_real);
	Mer::SystemFunction *input_string = new SystemFunction(Mem::BasicType::STRING, _input_string);
	void set_io()
	{
		cout->dnt_check_param();
		substr->set_param_types({Mer::Mem::BasicType::STRING, Mer::Mem::BasicType::INT, Mer::Mem::BasicType::INT });
		Mer::root_namespace->children.insert({ "std", mstd });
		mstd->set_new_func("substr", Mem::BasicType::STRING, substr);
		mstd->set_new_func("strsize", Mem::BasicType::INT, str_size);
		mstd->set_new_func("cout",Mem::BasicType::BVOID,cout);
		mstd->set_new_func("input_int", Mem::BasicType::INT,input_int);
		mstd->set_new_func("input_real", Mem::BasicType::DOUBLE, input_real);
		mstd->set_new_func("input_string", Mem::BasicType::INT, input_string);
	}
}