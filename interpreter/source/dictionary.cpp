#include "../include/clib/dictionary.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
namespace Mer
{
	using __member_function = std::function<Mem::Object(const std::vector<Mem::Object>&)>;
	void _register_member_function
	(std::string name, size_t type, size_t ret_type, const std::vector<size_t>& param_list, __member_function mf);
	extern std::vector<Mem::Object> parents_vec;

	//set method
	namespace {
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
	}
	namespace Container
	{
		size_t Set::type_code = 0;
		Set::Set(size_t element_type) :data(find_compare_operator(element_type))
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
	}
	_compare_operator find_compare_operator(size_t s)
	{
		auto result = compare_map.find(s);
		// it should checked at parser phase.
		if (result == compare_map.end())
			throw std::runtime_error("merdog is broken!");
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
		size_t tc = Container::Set::type_code;
		this_namespace->sl_table->push("set", ctr);
		Mem::container_register.insert({ tc,register_set });
		Mem::type_index.insert({ "set" ,tc });
		auto vec_type = new Mem::Type("set", tc, { tc });
		vec_type->type_kind = Mem::Type::container;
		Mem::type_map.insert({ tc,
			 vec_type });
	}
	void register_set(size_t element_type)
	{
		size_t cur_type = Mem::merge(Container::Set::type_code, element_type);
		type_init_map[cur_type] = std::make_shared<Container::Set>(element_type);
		auto set_type = new Mem::Type("set<" + type_to_string(element_type) + ">", cur_type, { cur_type });
		set_type->type_kind = Mem::Type::container;
		Mem::type_map.insert({ cur_type, set_type });

		if (compare_map.find(element_type) == compare_map.end())
			throw Error("type set<" + type_to_string(element_type) + ">" + " don't exist compare operator!");
		Mem::type_op_type_map.insert({ cur_type, {{"[]",element_type}} });
		_register_member_function("insert", cur_type, Mem::BVOID, { element_type }, _set_insert);
		_register_member_function("size", cur_type, Mem::INT, {  }, _set_size);
		_register_member_function("exists", cur_type, Mem::BOOL, { element_type }, _set_exists);
		_register_member_function("erase", cur_type, Mem::BOOL, { element_type },_set_erase);
	}
}