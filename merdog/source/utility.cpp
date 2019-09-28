#include "../include/clib/utility.hpp"
#include "../include/memory.hpp"
#include <time.h>
#include <random>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
namespace Mer
{
	namespace
	{
		Mem::Object _time_record(std::vector<Mem::Object>& args)
		{
			return std::make_shared<Mem::Double>(clock()*1000.0L);
		}
		Mem::Object _random_int(std::vector<Mem::Object>& args)
		{
			int range_begin= std::static_pointer_cast<Mem::Int>(args[0])->get_value();
			int range_end = std::static_pointer_cast<Mem::Int>(args[1])->get_value();
			static std::uniform_int_distribution<int> distributor(range_begin,range_end);
			static std::default_random_engine e(clock());
			return std::make_shared<Mem::Int>(distributor(e));
		}
		Mem::Object _alloc_heap(std::vector<Mem::Object>& args)
		{
			mem.check_heap();
			return nullptr;
		}
		Mem::Object _sleep(std::vector<Mem::Object>& args)
		{
			int time = std::static_pointer_cast<Mem::Int>(args[0])->get_value();
#ifdef _WIN32
			Sleep(time);
#else
			::sleep(time);
#endif
			return nullptr;
		}
		Mem::Object _get_type(std::vector<Mem::Object>& args)
		{
			return std::make_shared<Mem::Int>(args[1]->get_type());
		}
		Mem::Object _system(std::vector<Mem::Object>& args)
		{
			std::string str = args[0]->to_string();
			::system(str.c_str());
			return nullptr;
		}
	}
	SystemFunction* time_record = new SystemFunction(Mem::DOUBLE, _time_record);
	SystemFunction* random_int=new SystemFunction(Mem::INT,_random_int);
	SystemFunction* sleep = new SystemFunction(Mem::BVOID, _sleep);
	SystemFunction* csystem = new SystemFunction(Mem::BVOID, _system);
	SystemFunction* alloc_heap= new SystemFunction(Mem::BVOID, _alloc_heap);
	SystemFunction* type = new SystemFunction(Mem::BVOID, _get_type);
}

void Mer::set_utility()
{
	mstd->set_new_func("clock", Mer::Mem::DOUBLE, Mer::time_record);
	mstd->set_new_func("rand_int", Mer::Mem::INT, Mer::random_int);
	mstd->set_new_func("sleep", Mer::Mem::BVOID, Mer::sleep);
	root_namespace->set_new_func("type", Mer::Mem::BVOID, Mer::type);
	root_namespace->set_new_func("system", Mer::Mem::BVOID, Mer::csystem);
	root_namespace->set_new_func("_heap_mem_check", Mer::Mem::BVOID, Mer::alloc_heap);
	random_int->set_param_types({ Mer::Mem::BasicType::INT, Mer::Mem::BasicType::INT });
	sleep->set_param_types({ Mer::Mem::BasicType::INT });
	type->dnt_check_param();
	csystem->set_param_types({ Mer::Mem::BasicType::STRING });
}