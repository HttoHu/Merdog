#include "../include/clib/merdog_io.hpp"

namespace Mer
{
	namespace
	{
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
	Mer::SystemFunction *cout=new SystemFunction(Mem::BasicType::VOID,_cout);
	Mer::SystemFunction *input_int=new SystemFunction(Mem::BasicType::INT, _input_int);
	Mer::SystemFunction *input_real = new SystemFunction(Mem::BasicType::DOUBLE, _input_real);
	Mer::SystemFunction *input_string = new SystemFunction(Mem::BasicType::STRING, _input_string);
	void set_io()
	{
		Mer::root_namespace->children.insert({ "std", mstd });
		mstd->set_new_func("cout",Mem::BasicType::VOID,cout);
		mstd->set_new_func("input_int", Mem::BasicType::INT,input_int);
		mstd->set_new_func("input_real", Mem::BasicType::DOUBLE, input_real);
		mstd->set_new_func("input_string", Mem::BasicType::INT, input_string);
	}
}