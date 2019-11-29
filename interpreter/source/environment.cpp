/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include "../include/environment.hpp"
#include "../include/parser.hpp"
#include "../include/clib/maths.hpp"
#include "../include/clib/containers.hpp"
#include "../include/clib/dictionary.hpp"
#include "../include/clib/utility.hpp"
#include "../include/function.hpp"
#include "../include/memory.hpp"
std::map<std::string, void(*)()> Mer::repository{
	{"vector",Mer::Container::using_vector},{"deque",Mer::Container::using_deque},
	{"set",Mer::using_set},{"map",Mer::using_map}
};
std::vector<size_t*> Mer::_pcs;
std::string Mer::output_buff = "";
std::vector<std::pair<Mer::PosPtr, Mer::PosPtr>> Mer::_nearest_loop_pos;
std::vector<Mer::ParserNode*> Mer::structure_parent_stack;


namespace Mer
{
	extern std::map<type_code_index, std::map<std::string, Mer::FunctionBase*>> member_function_table;
	namespace Mem
	{
		void _clear_type_info();
	}
	namespace
	{
		void clear()
		{
			Mer::token_stream.clear();

			delete Mer::root_namespace;
			Mer::root_namespace = new Mer::Namespace(nullptr);
			delete mstd;
			mstd = new Mer::Namespace(nullptr);

			Mer::this_namespace = Mer::root_namespace;
			std::vector<FunctionBase*> vec;
			for (auto a : rem_functions)
			{
				delete a;
			}
			Mer::rem_functions.clear();
			Mer::pre_stmt.clear();

			Mer::mem.get_current() = 0;
			Mer::mem.get_index() = 0;

			Mem::_clear_type_info();

			compare_map.clear();

			// remove initialization function
			for (auto a : type_init_function_map)
				delete a.second;
	
			// remove member function call
			for (auto& a : member_function_table)
			{
				for (auto& b : a.second)
				{
					delete b.second;
				}
			}
		}
		void _merdog_init_()
		{
			using namespace Mer;
			using namespace Mem;
			Mer::root_namespace->children.insert({ "std", mstd });
			auto compare_op = [](Mem::Object a, Mem::Object b) {
				return std::static_pointer_cast<Mem::Bool>(a->operator<(b))->_value(); };
			compare_map.insert({ Mem::INT,compare_op });
			compare_map.insert({ Mem::BOOL,compare_op });
			compare_map.insert({ Mem::DOUBLE,compare_op });
			compare_map.insert({ Mem::STRING,compare_op });
			compare_map.insert({ Mem::CHAR,compare_op });
			Mer::set_io();
			Mer::set_maths();
			Mer::set_utility();
		}
	}

	std::string run_interpreter(const std::string& file_content)
	{
		output_buff = "";

		_merdog_init_();
		try
		{
			Mer::build_token_stream(file_content);
			Parser::program()->execute();
			clear();

		}
		catch (std::exception & e)
		{
			auto ret = e.what();
			token_stream.clear();
			clear();
			return ret;
		}
		catch (Mer::Error & e)
		{
			auto ret = e.what();
			std::cout << ret;
			token_stream.clear();
			clear();
			return ret;
		}
		return output_buff;
	}
	void new_loop(PosPtr a, PosPtr b)
	{
		_nearest_loop_pos.push_back(std::make_pair(a, b));
	}
	void end_loop()
	{
		_nearest_loop_pos.pop_back();
	}
	Mer::PosPtr& loop_start()
	{
		return _nearest_loop_pos.back().first;
	}
	Mer::PosPtr& loop_end()
	{
		return _nearest_loop_pos.back().second;
	}

}
