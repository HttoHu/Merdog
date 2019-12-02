#include "../include/clib/utility.hpp"
#include "../include/memory.hpp"
#include <time.h>
#include <random>
#include <cassert>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
namespace Mer
{
	namespace
	{
		int my_string_to_int(const std::string& str)
		{
			int ret = 0;
			for (int i = 0; i < str.size(); i++)
			{
				if (isdigit(str[i]))
				{
					ret = ret * 10 + (str[i] - 48);
				}
				else
				{
					break;
				}
			}
			return ret;
		}
		double my_string_to_double(const std::string& str)
		{
			int ret = 0;
			int pos = 0;
			for (; pos < str.size(); pos++)
			{
				if (isdigit(str[pos]))
				{
					ret = ret * 10 + (str[pos] - 48);
				}
				else
				{
					break;
				}
			}
			double tmp = 0.0;
			double tmp2 = 1;
			if (str[pos] == '.')
			{
				pos++;
				for (; pos < str.size(); pos++)
				{
					if (isdigit(str[pos]))
					{
						tmp2 /= 10.0;
						tmp +=tmp2 * (str[pos] - (size_t)48);
					}
				}
			}
			return (double)ret + tmp;
		}




		Mem::Object _time_record(const std::vector<Mem::Object>& args)
		{
			return std::make_shared<Mem::Int>(clock());
		}
		Mem::Object _random_int(const std::vector<Mem::Object>& args)
		{
			int range_begin= std::static_pointer_cast<Mem::Int>(args[0])->get_value();
			int range_end = std::static_pointer_cast<Mem::Int>(args[1])->get_value();
			static std::uniform_int_distribution<int> distributor(range_begin,range_end);
			static std::default_random_engine e(clock());
			return std::make_shared<Mem::Int>(distributor(e));
		}
		Mem::Object _sleep(const std::vector<Mem::Object>& args)
		{
			int time = std::static_pointer_cast<Mem::Int>(args[0])->get_value();
#ifdef _WIN32
			Sleep(time);
#else
			::sleep(time);
#endif
			return nullptr;
		}
		Mem::Object _to_string(const std::vector<Mem::Object>& args)
		{
			return std::make_shared<Mem::String>(args[0]->to_string());
		}
		Mem::Object _mer_exit(const std::vector<Mem::Object>& args)
		{
			std::exit(1);
			return nullptr;
		}
		Mem::Object _system(const std::vector<Mem::Object>& args)
		{
			std::string str = args[0]->to_string();
			::system(str.c_str());
			return nullptr;
		}
		Mem::Object _convert_to_int(const std::vector<Mem::Object>& args)
		{
			std::string str = args[0]->to_string();
			return std::make_shared<Mem::Int>(my_string_to_int(str));
		}
		Mem::Object _convert_to_real(const std::vector<Mem::Object>& args)
		{
			std::string str = args[0]->to_string();
			return std::make_shared<Mem::Double>(my_string_to_double(str));
		}
	}

}
void Mer::set_utility()
{
	SystemFunction* time_record = new SystemFunction(Mem::INT, _time_record);
	SystemFunction* random_int = new SystemFunction(Mem::INT, _random_int);
	SystemFunction* sleep = new SystemFunction(Mem::BVOID, _sleep);
	SystemFunction* csystem = new SystemFunction(Mem::BVOID, _system);
	SystemFunction* my_to_string = new SystemFunction(Mem::STRING, _to_string);
	SystemFunction* to_int = new SystemFunction(Mem::INT, _convert_to_int);
	SystemFunction* to_real = new SystemFunction(Mem::DOUBLE, _convert_to_real);
	SystemFunction* mer_exit = new SystemFunction(Mem::BVOID, _mer_exit);
	//======================================================================================
	random_int->set_param_types({ Mer::Mem::BasicType::INT, Mer::Mem::BasicType::INT });
	sleep->set_param_types({ Mer::Mem::BasicType::INT });
	my_to_string->dnt_check_param();
	csystem->set_param_types({ Mer::Mem::BasicType::STRING });
	to_int->set_param_types({ Mer::Mem::BasicType::STRING });
	to_real->set_param_types({ Mer::Mem::BasicType::STRING });
	mer_exit->set_param_types({ });
	//======================================================================================
	mstd->set_new_func("clock",  time_record);
	mstd->set_new_func("rand_int",  random_int);
	mstd->set_new_func("sleep",  sleep);
	root_namespace->set_new_func("exit", mer_exit);
	root_namespace->set_new_func("to_string", my_to_string);
	root_namespace->set_new_func("system",  csystem);
	root_namespace->set_new_func("to_int", to_int);
	root_namespace->set_new_func("to_real", to_real);

}