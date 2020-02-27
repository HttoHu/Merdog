#include "../include/clib/dictionary.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
namespace Mer
{
	using __member_function = std::function<Mem::Object(const std::vector<Mem::Object>&)>;
	void _register_member_function
	(std::string name, type_code_index type, type_code_index ret_type, const std::vector<type_code_index>& param_list, __member_function mf);

	void _register_type_init
	(type_code_index type, const std::vector<type_code_index>& param_list, __member_function mf);

	extern std::vector<Mem::Object> parents_vec;

	//set method
	namespace {
		Mem::Object _set_list_init(const std::vector<Mem::Object>& args) {
			auto list = std::static_pointer_cast<Mem::InitListObj> (args[0]);
			return std::make_shared<Container::Set>(list->get_ele_type(),std::vector<Mem::Object>(list->elems.begin(), list->elems.end()));
		}
		Mem::Object _set_insert(const std::vector<Mem::Object>& args)
		{
			auto& set_content = std::static_pointer_cast<Container::Set>(parents_vec.back())->data;
			set_content.insert(args[0]);
			return nullptr;
		}
		Mem::Object _set_size(const std::vector < Mem::Object >& args)
		{
			auto& set_content = std::static_pointer_cast<Container::Set>(parents_vec.back())->data;
			return std::make_shared<Mem::Int>(set_content.size());
		}
		Mem::Object _set_exists(const std::vector<Mem::Object>& args)
		{
			auto& set_content = std::static_pointer_cast<Container::Set>(parents_vec.back())->data;
			auto result = set_content.find(args[0]);
			if (result == set_content.end())
				return std::make_shared<Mem::Bool>(false);
			return std::make_shared<Mem::Bool>(true);
		}

		Mem::Object _set_clear(const std::vector<Mem::Object>& args)
		{
			auto& set_content = std::static_pointer_cast<Container::Set>(parents_vec.back())->data;
			set_content.clear();
			return nullptr;
		}
		Mem::Object _set_erase(const std::vector < Mem::Object >& args)
		{
			auto& set_content = std::static_pointer_cast<Container::Set>(parents_vec.back())->data;
			auto result = set_content.find(args[0]);
			if (result == set_content.end()) {
				return std::make_shared<Mem::Bool>(false);
			}
			set_content.erase(result);
			return std::make_shared<Mem::Bool>(true);
		}
		Mem::Object _set_index_by_number(const std::vector < Mem::Object >& args)
		{
			auto& set_content = std::static_pointer_cast<Container::Set>(parents_vec.back())->data;
			int pos = Mem::get_raw<int>(args[0]);
			auto tmp = set_content.begin();
			// tmp+=pos is illegal of C++
			for (int i = 0; i < pos; i++)
				tmp++;
			return *tmp;
		}

	}
	//map method
	namespace {
		Mem::Object _map_insert(const std::vector<Mem::Object>& args)
		{
			auto& map_content = std::static_pointer_cast<Container::Map>(parents_vec.back())->data;
			map_content.insert({ args[0],args[1] });
			return nullptr;
		}
		Mem::Object _map_exists(const std::vector<Mem::Object>& args)
		{
			auto& map_content = std::static_pointer_cast<Container::Map>(parents_vec.back())->data;
			auto result = map_content.find(args[0]);
			if (result == map_content.end())
				return std::make_shared<Mem::Bool>(false);
			return std::make_shared<Mem::Bool>(true);
		}
		Mem::Object _map_erase(const std::vector < Mem::Object >& args)
		{
			auto& map_content = std::static_pointer_cast<Container::Map>(parents_vec.back())->data;
			auto result = map_content.find(args[0]);
			if (result == map_content.end()) {
				return std::make_shared<Mem::Bool>(false);
			}
			map_content.erase(result);
			return std::make_shared<Mem::Bool>(true);
		}
		Mem::Object _map_size(const std::vector < Mem::Object >& args)
		{
			auto& map_content = std::static_pointer_cast<Container::Map>(parents_vec.back())->data;
			return std::make_shared<Mem::Int>(map_content.size());
		}
		Mem::Object _map_clear(const std::vector < Mem::Object >& args)
		{
			auto& map_content = std::static_pointer_cast<Container::Map>(parents_vec.back())->data;
			map_content.clear();
			return nullptr;

		}
		Mem::Object _map_index_by_number(const std::vector < Mem::Object >& args)
		{
			auto& map_content = std::static_pointer_cast<Container::Map>(parents_vec.back())->data;
			int pos = Mem::get_raw<int>(args[0]);
			auto tmp = map_content.begin();
			// tmp+=pos is illegal of C++
			for (int i = 0; i < pos; i++)
				tmp++;
			return tmp->second;
		}
	}
	namespace Container
	{
		type_code_index Map::type_code=0;
		type_code_index Set::type_code = 0;
		Set::Set(type_code_index element_type, const std::vector<Mem::Object>& vec):Set(element_type)
		{
			for (const auto& a : vec)
				data.insert(a->clone());
		}
		Set::Set(type_code_index element_type) :data(find_compare_operator(element_type))
		{
		}
		Mem::Object Set::operator[](Mem::Object index)
		{
			auto ret = data.find(index);
			return *ret;
		}
		Mem::Object Set::clone() const
		{
			return std::make_shared<Set>(data);
		}
		Map::Map(type_code_index key) :data(find_compare_operator(key)) {}

		Mem::Object Map::operator[](Mem::Object index)
		{
			auto ret = data.find(index);
			return ret->second;
		}
		Mem::Object Map::clone() const
		{
			return std::make_shared<Map>(data);
		}
	}
	_compare_operator find_compare_operator(type_code_index s)
	{
		auto result = compare_map.find(s);

		// it should checked at parser phase.
		if (result == compare_map.end())
		{
			throw Error(type_to_string(s)+" doesn't have compare operator!");
		}
		return result->second;
	}
	void using_set()
	{
		// when you create an ContainerTypeRecorder the type_counter will be added two. So you need to sub 2 to
		// get the vector's type;
		auto compare_op = [](Mem::Object a, Mem::Object b) {
			return std::static_pointer_cast<Mem::Bool>(a->operator<(b))->_value();
		};


		auto ctr = new ContainerTypeRecorder("set");
		Container::Set::type_code = ctr->get_type();
		type_code_index tc = Container::Set::type_code;
		this_namespace->sl_table->push("set", ctr);
		Mem::container_register.insert({ tc,register_set });
		Mem::type_index.insert({ "set" ,tc });
		auto set_type = new Mem::Type("set", tc, { tc });
		set_type->type_kind = Mem::Type::kind::container;
		Mem::type_map.insert({ tc,
			 set_type });
	}
	void register_set(type_code_index element_type)
	{
		type_code_index cur_type = Mem::merge(Container::Set::type_code, element_type);
		type_init_map[cur_type] = std::make_shared<Container::Set>(element_type);
		auto set_type = new Mem::Type("set<" + type_to_string(element_type) + ">", cur_type, { cur_type });
		set_type->type_kind = Mem::Type::kind::container;
		Mem::type_map.insert({ cur_type, set_type });

		if (compare_map.find(element_type) == compare_map.end())
			throw Error("type set<" + type_to_string(element_type) + ">" + " don't exist compare operator!");
		_register_type_init(cur_type, { Mem::INIT_LIST }, _set_list_init);
		_register_member_function("insert", cur_type, Mem::BVOID, { element_type }, _set_insert);
		_register_member_function("size", cur_type, Mem::INT, {  }, _set_size);
		_register_member_function("clear", cur_type, Mem::INT,{}, _set_clear);
		_register_member_function("exists", cur_type, Mem::BOOL, { element_type }, _set_exists);
		_register_member_function("erase", cur_type, Mem::BOOL, { element_type }, _set_erase);
		_register_member_function("pos_visit", cur_type, element_type, { Mem::INT }, _set_index_by_number);
	}
	void register_map(type_code_index element_type)
	{
		std::pair<type_code_index, type_code_index> pair_type = Mem::demerge(element_type);
		type_code_index key_type = pair_type.first;
		type_code_index value_type = pair_type.second;
		type_code_index cur_type = Mem::merge(Container::Map::type_code, element_type);
		type_init_map[cur_type] = std::make_shared<Container::Map>(key_type);
		auto map_type = new Mem::Type("map<" + type_to_string(key_type) + "," + type_to_string(value_type) + ">", cur_type, { cur_type });
		map_type->type_kind = Mem::Type::kind::container;
		Mem::type_map.insert({ cur_type, map_type });

		if (compare_map.find(key_type) == compare_map.end())
			throw Error("type map<" + type_to_string(key_type) + "," + type_to_string(value_type) + ">" + " don't exist compare operator!");
		Mem::type_op_type_map.insert({ cur_type, {{"[]",value_type}} });
		_register_member_function("insert", cur_type, Mem::BVOID, { key_type,value_type }, _map_insert);
		_register_member_function("size", cur_type, Mem::INT, {  }, _map_size);
		_register_member_function("clear", cur_type, Mem::INT,{}, _map_clear);
		_register_member_function("exists", cur_type, Mem::BOOL, { key_type }, _map_exists);
		_register_member_function("erase", cur_type, Mem::BOOL, { key_type }, _map_erase);
		_register_member_function("pos_visit", cur_type, element_type, { Mem::INT }, _map_index_by_number);
	}
	void using_map()
	{
		auto compare_op = [](Mem::Object a, Mem::Object b) {
			return std::static_pointer_cast<Mem::Bool>(a->operator<(b))->_value();
		};


		auto ctr = new ContainerTypeRecorder("map");
		Container::Map::type_code = ctr->get_type();
		type_code_index tc = Container::Map::type_code;
		this_namespace->sl_table->push("map", ctr);
		Mem::container_register.insert({ tc,register_map });
		Mem::type_index.insert({ "map" ,tc });
		auto map_type = new Mem::Type("map", tc, { tc });
		map_type->type_kind = Mem::Type::kind::container;
		Mem::type_map.insert({ tc,
			 map_type });
	}


}