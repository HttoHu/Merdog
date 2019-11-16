/*
* MIT License
* Copyright (c) 2019 Htto Hu
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
#include "../include/basic_objects.hpp"
#include "../include/environment.hpp"
#include "../include/clib/merdog_io.hpp"
#include "../include/compound_box.hpp"
#include "../include/word_record.hpp"
#include <fstream>
#include <filesystem>
namespace Mer
{
	namespace
	{
		std::shared_ptr<Mem::Int> _make_int_obj(int n)
		{
			return std::make_shared<Mem::Int>(n);
		}
		Mem::Object _init_str_n(std::vector<Mem::Object>& args)
		{
			int count = Mem::get_raw<int>(args[0]);
			char c = std::static_pointer_cast<Mem::Char>(args[1])->get_value();
			return std::make_shared<Mem::String>(std::string(count, c));
		}
		Mem::Object _substr(std::vector<Mem::Object>& args)
		{
			auto tmp = std::dynamic_pointer_cast<Mem::String>(args[0]);
			auto off = std::dynamic_pointer_cast<Mem::Int>(args[1]);
			auto size = std::dynamic_pointer_cast<Mem::Int>(args[2]);
			return std::make_shared<Mem::String>(tmp->to_string().substr(off->get_value(), size->get_value()));
		}
		Mem::Object _str_size(std::vector<Mem::Object>& args)
		{
			auto tmp = std::static_pointer_cast<Mem::String>(args[0]);
			return std::make_shared<Mem::Int>(tmp->to_string().size());
		}
		Mem::Object _cout(std::vector<Mem::Object>& args)
		{
			for (const auto& a : args)
#ifndef DISABLEIO
				std::cout << a->to_string();
#else
				output_buff += a->to_string();
#endif
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
		Mem::Object _input_char(std::vector<Mem::Object>& args)
		{
			if (args.size() != 0)
				throw Error("argument size error");
			char obj;
			std::cin >> obj;
			return std::make_shared<Mem::Char>(obj);
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
			std::string obj = "";
			std::cin >> obj;
			return std::make_shared<Mem::String>(obj);
		}
#ifndef DISABLEIO
		Mem::Object _open(std::vector<Mem::Object>& args)
		{
			auto arg1 = args[0];
			auto ss = std::static_pointer_cast<USObject>(arg1);
			std::ifstream ifs;
			ifs.open(args[1]->to_string());
			if (!std::filesystem::exists(args[1]->to_string()))
				throw std::runtime_error("file " + args[1]->to_string() + " don't exist");
			(*ss)[_make_int_obj(0)]->operator=(args[1]->clone());
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
			auto arg1 = args[0];
			auto ss = std::static_pointer_cast<USObject>(arg1);
			return std::make_shared<Mem::String>((*std::static_pointer_cast<Mem::AnyObj>((*ss)[std::make_shared<Mem::Int>(1)])).
				cast<std::vector<std::string>>()[Mem::get_raw<int>(args[1])]);
		}
		Mem::Object _set_line(std::vector<Mem::Object>& args)
		{
			auto ss = std::static_pointer_cast<USObject>(args[0]);
			auto content = std::static_pointer_cast<Mem::AnyObj>(ss->operator[](std::make_shared<Mem::Int>(1)));
			auto& file_content = content->cast<std::vector<std::string>>();
			file_content[std::static_pointer_cast<Mem::Int>(args[1])->get_value()] = args[2]->to_string();
			return nullptr;
		}
		Mem::Object _insert_line(std::vector<Mem::Object>& args)
		{
			auto fobj = std::static_pointer_cast<USObject>(args[0]);
			int line_no = Mem::Int::get_val(args[1]);
			auto content = std::static_pointer_cast<Mem::AnyObj>(fobj->operator[](_make_int_obj(1)));
			auto& file_content = content->cast<std::vector<std::string>>();
			std::string insert_content = args[2]->to_string();
			file_content.insert(file_content.begin() + line_no, insert_content);
			return nullptr;
		}
		Mem::Object _write_into_file(std::vector<Mem::Object>& args)
		{
			auto fobj = std::static_pointer_cast<USObject>(args[0]);
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
			auto obj = std::static_pointer_cast<USObject>(args[0]);
			auto ret = std::static_pointer_cast<Mem::AnyObj>((obj->operator[](_make_int_obj(1))))->cast<std::vector<std::string>>().size();
			return _make_int_obj(ret);
		}
		Mem::Object _exists_file(std::vector<Mem::Object>& args)
		{
			return std::make_shared<Mem::Bool>(std::filesystem::exists(args[0]->to_string()));
		}
#endif 
	}
	Namespace* mstd = new Namespace(nullptr);
	Mer::SystemFunction* substr = new SystemFunction(Mem::BasicType::STRING, _substr);
	Mer::SystemFunction* str_size = new SystemFunction(Mem::BasicType::INT, _str_size);
	Mer::SystemFunction* cout = new SystemFunction(Mem::BasicType::BVOID, _cout);
	Mer::SystemFunction* input_int = new SystemFunction(Mem::BasicType::INT, _input_int);
	Mer::SystemFunction* input_real = new SystemFunction(Mem::BasicType::DOUBLE, _input_real);
	Mer::SystemFunction* input_string = new SystemFunction(Mem::BasicType::STRING, _input_string);
	Mer::SystemFunction* input_char = new SystemFunction(Mem::BasicType::CHAR, _input_char);
#ifndef DISABLEIO
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
		cout->dnt_check_param();
		substr->set_param_types({ Mer::Mem::BasicType::STRING, Mer::Mem::BasicType::INT, Mer::Mem::BasicType::INT });
		str_size->set_param_types({ Mer::Mem::BasicType::STRING });
		Mer::root_namespace->children.insert({ "std", mstd });
		// set string
		member_function_table[Mem::STRING]["substr"] = substr;
		member_function_table[Mem::STRING]["size"] = str_size;
		// string init
		auto str_init = new SystemFunction(Mem::STRING, _init_str_n);
		str_init->set_param_types({ Mem::INT,Mem::CHAR });
		type_init_function_map[InitKey(Mem::STRING, std::vector<size_t>{ Mem::INT, Mem::CHAR })] = str_init;
		mstd->set_new_func("cout", cout);
		mstd->set_new_func("input_int", input_int);
		mstd->set_new_func("input_real", input_real);
		mstd->set_new_func("input_char", input_char);
		mstd->set_new_func("input_string", input_string);
#ifndef DISABLEIO
		auto exists_file = new SystemFunction(Mem::BOOL, _exists_file);
		exists_file->set_param_types({ Mem::STRING });
		//file_stream
		set_file_operator_class();
		mstd->set_new_func("exist_file", exists_file);
#endif


	}
}