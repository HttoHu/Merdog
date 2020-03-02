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
	void _register_internal_function
	(std::string name, type_code_index ret_type, const std::vector<type_code_index>& param_list, std::function<Mem::Object(const std::vector<Mem::Object>&)> mf, Namespace* _nsp = this_namespace);
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
		Mem::Object _isdigit(const std::vector<Mem::Object> &args) {
			char ch = std::static_pointer_cast<Mem::Char>(args[0])->get_value();
			return std::make_shared<Mem::Bool>(isdigit(ch));
		}
		Mem::Object _isalpha(const std::vector<Mem::Object> &args) {
			char ch = std::static_pointer_cast<Mem::Char>(args[0])->get_value();
			return std::make_shared<Mem::Bool>(isalpha(ch));
		}
		Mem::Object _isalnum(const std::vector<Mem::Object> &args) {
			char ch = std::static_pointer_cast<Mem::Char>(args[0])->get_value();
			return std::make_shared<Mem::Bool>(isalnum(ch));
		}
		Mem::Object _max(const std::vector<Mem::Object> &args) {
			return (*args[0])<args[1] ? args[1]->clone():args[0]->clone();
		}
	}

}
void Mer::set_utility()
{
	SystemFunction* my_to_string = new SystemFunction(Mem::STRING, _to_string);
	//======================================================================================
	my_to_string->dnt_check_param();
	//======================================================================================
	_register_internal_function("clock", Mem::INT, {}, _time_record, mstd);
	_register_internal_function("rand_int", Mem::INT, {Mem::INT,Mem::INT}, _random_int, mstd);
	_register_internal_function("sleep", Mem::BVOID, {Mem::INT}, _sleep, mstd);
	_register_internal_function("exit", Mem::BVOID, {}, _mer_exit);
	_register_internal_function("system", Mem::BVOID, {Mem::STRING}, _system);
	_register_internal_function("isdigit", Mem::BOOL, { Mem::CHAR }, _isdigit);
	_register_internal_function("isalpha", Mem::BOOL, { Mem::CHAR }, _isalpha);
	_register_internal_function("isalnum", Mem::BOOL, { Mem::CHAR }, _isalnum);
	_register_internal_function("to_int", Mem::INT, {Mem::STRING}, _convert_to_int);
	_register_internal_function("max", Mem::DOUBLE, { Mem::DOUBLE,Mem::DOUBLE }, _max);
	_register_internal_function("max", Mem::INT, { Mem::INT,Mem::INT }, _max);
	_register_internal_function("to_real", Mem::DOUBLE, { Mem::STRING }, _convert_to_real);
	root_namespace->set_new_func("to_string", my_to_string);

}