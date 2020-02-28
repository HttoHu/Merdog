/*
	MIT License

	Copyright (c) 2019 HttoHu

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

*/
// file_stream
/*
 mem:struct
 - 0: filename
 - 1: std::vector<std::string> file_content;
 --------------------
 method
 => open(string), open a file by its name
 => string read(int), read line_no text;
 => void write(), write the content to the file.
*/
#include <sstream>
#include <algorithm>
#include "../include/basic_objects.hpp"
#include "../include/environment.hpp"
#include "../include/clib/merdog_io.hpp"
#include "../include/compound_box.hpp"
#include "../include/word_record.hpp"
#include <fstream>
#ifdef USING_CXX17
#include <filesystem>
#endif
namespace Mer
{

	void _register_internal_function
	(std::string name,  type_code_index ret_type, const std::vector<type_code_index>& param_list, std::function<Mem::Object(const std::vector<Mem::Object>&)> mf,Namespace* _nsp=this_namespace) {
		SystemFunction* tmp = new SystemFunction(ret_type, mf);
		tmp->set_param_types(param_list);
		_nsp->set_new_func(name, tmp);
	}
	namespace
	{
		std::shared_ptr<Mem::Int> _make_int_obj(int n)
		{
			return std::make_shared<Mem::Int>(n);
		}
		Mem::Object _init_str_n(const std::vector<Mem::Object>& args)
		{
			int count = Mem::get_raw<int>(args[0]);
			char c = std::static_pointer_cast<Mem::Char>(args[1])->get_value();
			return std::make_shared<Mem::String>(std::string(count, c));
		}
		Mem::Object _str_find_str(const std::vector<Mem::Object>& args) {
			auto tmp = std::static_pointer_cast<Mem::String>(parents_vec.back())->str;
			std::string str = std::static_pointer_cast<Mem::String> (args[0])->str;
			int spos = std::static_pointer_cast<Mem::Int>(args[1])->get_value();
			return std::make_shared<Mem::Int>(tmp.find(str,spos));
		}
		Mem::Object _substr(const std::vector<Mem::Object>& args)
		{
			auto tmp = std::static_pointer_cast<Mem::String>(parents_vec.back());
			auto off = std::static_pointer_cast<Mem::Int>(args[0]);
			auto size = std::static_pointer_cast<Mem::Int>(args[1]);
			return std::make_shared<Mem::String>(tmp->to_string().substr(off->get_value(), size->get_value()));
		}
		Mem::Object _str_size(const std::vector<Mem::Object>& args)
		{
			auto tmp = std::static_pointer_cast<Mem::String>(parents_vec.back());
			return std::make_shared<Mem::Int>(tmp->to_string().size());
		}
		Mem::Object _str_find(const std::vector<Mem::Object> &args) {
			auto tmp = std::static_pointer_cast<Mem::String>(parents_vec.back());
			std::string tmp_str = tmp->str;
			char ch = std::static_pointer_cast<Mem::Char>(args[0])->get_value();
			int startPos = std::static_pointer_cast<Mem::Int>(args[1])->get_value();
			return std::make_shared<Mem::Int>(tmp->str.find(ch, startPos));
		}
		Mem::Object _str_to_lower_case(const std::vector<Mem::Object>& args) {
			std::string str = std::static_pointer_cast<Mem::String>(parents_vec.back())->str;
			std::transform(str.begin(), str.end(), str.begin(), tolower);
			return std::make_shared<Mem::String>(str);
		}
		Mem::Object _str_to_upper_case(const std::vector<Mem::Object>& args) {
			std::string str = std::static_pointer_cast<Mem::String>(parents_vec.back())->str;
			std::transform(str.begin(), str.end(), str.begin(), toupper);
			return std::make_shared<Mem::String>(str);
		}
		Mem::Object _cout(const std::vector<Mem::Object>& args)
		{
			for (const auto& a : args)
#ifndef DISABLEIO
				std::cout << a->to_string();
#else
				output_buff += a->to_string();
#endif
			return nullptr;
		}

		Mem::Object _input_int(const std::vector<Mem::Object>& args)
		{
			int obj = 0;
			if (input_buf.empty())
				std::cin >> obj;
			else
				my_stringstream >> obj;
			return std::make_shared<Mem::Int>(obj);
		}
		Mem::Object _input_char(const std::vector<Mem::Object>& args)
		{
			char obj;
			if (input_buf.empty())
				std::cin >> obj;
			else
				my_stringstream >> obj;
			return std::make_shared<Mem::Char>(obj);
		}
		Mem::Object _get_line(const std::vector<Mem::Object>& args)
		{
			std::string obj;
			if (input_buf.empty())
				std::getline(std::cin, obj);
			else
				std::getline(my_stringstream, obj);
			return std::make_shared<Mem::String>(obj);

		}
		Mem::Object _input_real(const std::vector<Mem::Object>& args)
		{

			double obj = 0;
			if (input_buf.empty())
				std::cin >> obj;
			else
				my_stringstream >> obj;
			return std::make_shared<Mem::Double>(obj);
		}
		Mem::Object _input_string(const std::vector<Mem::Object>& args)
		{

			std::string obj = "";
			if (input_buf.empty())
				std::cin >> obj;
			else
				my_stringstream >> obj;
			return std::make_shared<Mem::String>(obj);
		}
#ifdef USING_CXX17
		Mem::Object _open(std::vector<Mem::Object>& args)
		{
			auto arg1 = parents_vec.back();
			auto ss = std::static_pointer_cast<USObject>(arg1);
			std::ifstream ifs;
			ifs.open(args[0]->to_string());
			if (!std::filesystem::exists(args[0]->to_string()))
				throw std::runtime_error("file " + args[0]->to_string() + " don't exist");
			(*ss)[_make_int_obj(0)]->operator=(args[0]->clone());
			std::vector<std::string> str_vec;
			std::string tmp = "";
			while (std::getline(ifs, tmp))
				str_vec.push_back(tmp);
			ifs.close();
			*std::static_pointer_cast<Mem::AnyObj>((*ss)[std::make_shared<Mem::Int>(1)]) = std::move(str_vec);
			return nullptr;
		}
		Mem::Object _read_line(std::vector<Mem::Object>& args)
		{
			auto arg1 = parents_vec.back();
			auto ss = std::static_pointer_cast<USObject>(arg1);
			return std::make_shared<Mem::String>((*std::static_pointer_cast<Mem::AnyObj>((*ss)[std::make_shared<Mem::Int>(1)])).
				cast<std::vector<std::string>>()[Mem::get_raw<int>(args[0])]);
		}
		Mem::Object _set_line(std::vector<Mem::Object>& args)
		{
			auto ss = std::static_pointer_cast<USObject>(parents_vec.back());
			auto content = std::static_pointer_cast<Mem::AnyObj>(ss->operator[](std::make_shared<Mem::Int>(1)));
			auto& file_content = content->cast<std::vector<std::string>>();
			file_content[std::static_pointer_cast<Mem::Int>(args[0])->get_value()] = args[1]->to_string();
			return nullptr;
		}
		Mem::Object _insert_line(std::vector<Mem::Object>& args)
		{
			auto fobj = std::static_pointer_cast<USObject>(parents_vec.back());
			int line_no = Mem::Int::get_val(args[0]);
			auto content = std::static_pointer_cast<Mem::AnyObj>(fobj->operator[](_make_int_obj(1)));
			auto& file_content = content->cast<std::vector<std::string>>();
			std::string insert_content = args[1]->to_string();
			file_content.insert(file_content.begin() + line_no, insert_content);
			return nullptr;
		}
		Mem::Object _write_into_file(std::vector<Mem::Object>& args)
		{
			auto fobj = std::static_pointer_cast<USObject>(parents_vec.back());
			auto content = std::static_pointer_cast<Mem::AnyObj>(fobj->operator[](std::make_shared<Mem::Int>(1)));
			auto& file_content = content->cast<std::vector<std::string>>();
			std::string file_name = fobj->operator[](_make_int_obj(0))->to_string();
			if (!std::filesystem::exists(file_name))
				throw std::runtime_error("file " + file_name + " don't exist");
			std::ofstream of(file_name);
			of.clear();
			for (const auto& a : file_content)
			{
				of << a << std::endl;
			}
			of.close();
			return nullptr;
		}
		Mem::Object _line_count(std::vector<Mem::Object>& args)
		{
			auto obj = std::static_pointer_cast<USObject>(parents_vec.back());
			auto ret = std::static_pointer_cast<Mem::AnyObj>((obj->operator[](_make_int_obj(1))))->cast<std::vector<std::string>>().size();
			return _make_int_obj(ret);
		}
		Mem::Object _exists_file(std::vector<Mem::Object>& args)
		{
			return std::make_shared<Mem::Bool>(std::filesystem::exists(parents_vec.back()->to_string()));
		}
#endif
	}

	Namespace* mstd = new Namespace(nullptr);

#ifdef USING_CXX17
	void set_file_operator_class()
	{

		UStructure* filestream = new UStructure();
		std::string class_name = "file_stream";
		Mem::type_counter += 2;
		Mer::this_namespace->sl_table->push(class_name, new WordRecorder(ESymbol::SSTRUCTURE, Mem::type_counter));
		// set the structure of the class
		filestream->init_vec.push_back(std::make_shared<Mem::String>(""));
		filestream->init_vec.push_back(std::make_shared<Mem::AnyObj>(0));
		// register the information of file_stream
		ustructure_map.insert({ class_name,filestream });
		Mem::type_index.insert({ class_name,Mem::type_counter });
		type_name_mapping.insert({ Mem::type_counter,class_name });
		Mem::type_map.insert({ Mem::type_counter,new Mem::Type(class_name,Mem::type_counter,{size_t(Mem::type_counter)}) });
		// set opem method
		auto open_file = new SystemFunction(Mem::BasicType::BVOID, _open);
		open_file->set_param_types({ (size_t)Mem::type_counter,Mem::STRING });
		member_function_table[Mem::type_counter].insert({ "open",open_file });

		auto read_line = new SystemFunction(Mem::BasicType::STRING, _read_line);
		read_line->set_param_types({ (size_t)Mem::type_counter,Mem::INT });
		member_function_table[Mem::type_counter].insert({ "read",read_line });

		auto set_line = new SystemFunction(Mem::BasicType::BVOID, _set_line);
		set_line->set_param_types({ (size_t)Mem::type_counter,Mem::INT,Mem::STRING });
		member_function_table[Mem::type_counter].insert({ "set_line",set_line });

		auto insert = new SystemFunction(Mem::BVOID, _insert_line);
		insert->set_param_types({ (size_t)Mem::type_counter,Mem::INT,Mem::STRING });
		member_function_table[Mem::type_counter].insert({ "insert",insert });

		auto write_into_file = new SystemFunction(Mem::BVOID, _write_into_file);
		write_into_file->set_param_types({ (size_t)Mem::type_counter });
		member_function_table[Mem::type_counter].insert({ "write_into_file",write_into_file });

		auto line_count = new SystemFunction(Mem::BVOID, _line_count);
		line_count->set_param_types({ (size_t)Mem::type_counter });
		member_function_table[Mem::type_counter].insert({ "line_count",line_count });
}
#endif
	void set_io()
	{
		Mer::SystemFunction* substr = new SystemFunction(Mem::BasicType::STRING, _substr);
		Mer::SystemFunction* str_size = new SystemFunction(Mem::BasicType::INT, _str_size);
		Mer::SystemFunction* find_ch = new SystemFunction(Mem::BasicType::INT,_str_find);
		Mer::SystemFunction* find_str = new SystemFunction(Mem::BasicType::INT, _str_find_str);
		Mer::SystemFunction* to_lower = new SystemFunction(Mem::BasicType::STRING, _str_to_lower_case);
		Mer::SystemFunction* to_upper = new SystemFunction(Mem::BasicType::STRING, _str_to_upper_case);

		Mer::SystemFunction* cout = new SystemFunction(Mem::BasicType::BVOID, _cout);
		cout->dnt_check_param();
		// set string===========================================
		substr->set_param_types({ Mer::Mem::BasicType::INT, Mer::Mem::BasicType::INT });
		str_size->set_param_types({ });
		find_ch->set_param_types({ Mem::BasicType::CHAR,Mem::BasicType::INT });
		find_str->set_param_types({ Mem::BasicType::STRING,Mem::BasicType::INT});
		member_function_table[Mem::STRING]["substr"] = substr;
		member_function_table[Mem::STRING]["size"] = str_size;
		member_function_table[Mem::STRING]["find"] = find_ch;
		member_function_table[Mem::STRING]["find_str"] = find_str;
		member_function_table[Mem::STRING]["to_lower"] = to_lower;
		member_function_table[Mem::STRING]["to_upper"] = to_upper;
		// string init==================================================================
		auto str_init = new SystemFunction(Mem::STRING, _init_str_n);
		str_init->set_param_types({ Mem::INT,Mem::CHAR });
		type_init_function_map[InitKey(Mem::STRING, std::vector<type_code_index>{ Mem::INT, Mem::CHAR })] = str_init;
		//======================================================
		mstd->set_new_func("cout", cout);
		_register_internal_function("input_int", Mem::INT, {}, _input_int, mstd);
		_register_internal_function("input_char", Mem::CHAR, {}, _input_char, mstd);
		_register_internal_function("input_string", Mem::STRING, {}, _input_string, mstd);
		_register_internal_function("input_real", Mem::DOUBLE, {}, _input_real, mstd);
		_register_internal_function("getline", Mem::STRING, {}, _get_line, mstd);
#ifdef COMPILE_MERDOG_NEED_CXX17
		auto exists_file = new SystemFunction(Mem::BOOL, _exists_file);
		exists_file->set_param_types({ Mem::STRING });
		//file_stream
		set_file_operator_class();
		mstd->set_new_func("exist_file", exists_file);
#endif

	}
}