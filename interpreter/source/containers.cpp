#include "../include/clib/containers.hpp"
#include "../include/Namespace.hpp"
#include "../include/word_record.hpp"
#include "../include/compound_box.hpp"
namespace Mer
{
	//vector
	namespace
	{
		Mem::Object _init_vec(const std::vector < Mem::Object > & args)
		{
			int count = Mem::get_raw<int>(args[0]);
			return std::make_shared<Container::Vector>(count, args[1]);
		}
		Mem::Object _vec_push_back(const std::vector < Mem::Object >& args)
		{
			auto vec = std::static_pointer_cast<Container::Vector>(args[0]);
			vec->content.push_back(args[1]);
			return nullptr;
		}
		Mem::Object _vec_size(const std::vector < Mem::Object >& args)
		{
			auto vec = std::static_pointer_cast<Container::Vector>(args[0]);
			return std::make_shared<Mem::Int>(vec->content.size());
		}
	}
	namespace Container
	{
		int Vector::vector_type_code=0;
		Vector::Vector(int count, Mem::Object obj):content(count)
		{
			for (auto& a : content)
			{
				a = obj->clone();
			}
		}
		Mem::Object Vector::operator[](Mem::Object index)
		{
			
			return content[Mem::get_raw<int>(index)];
		}

		Mem::Object Vector::clone() const
		{
			return std::make_shared<Vector>(content);
		}

		void using_vector()
		{
			// when you create an ContainerTypeRecorder the type_counter will be added two. So you need to sub 2 to
			// get the vector's type;
			auto ctr = new ContainerTypeRecorder("vector");
			Vector::vector_type_code = ctr->get_type();
			this_namespace->sl_table->push("vector", ctr);
			Mem::container_register.insert({ Vector::vector_type_code,register_new_vector_type });
			Mem::type_index.insert({ "vector" ,Vector::vector_type_code });
			auto vec_type = new Mem::Type("vector", Vector::vector_type_code, { (size_t)Vector::vector_type_code });
			vec_type->type_kind = Mem::Type::container;
			Mem::type_map.insert({ Vector::vector_type_code,
				 vec_type });
		}

		void register_new_vector_type(size_t element_type)
		{
			// set init
			size_t cur_type = Mem::merge(Vector::vector_type_code, element_type);
			SystemFunction* init = new SystemFunction(cur_type, _init_vec);
			init->set_param_types({ Mem::INT, element_type });
			type_init_function_map[cur_type] = init;
			type_init_map[cur_type] = std::make_shared<Vector>();
			// set type
			auto vec_type = new Mem::Type("vector", cur_type, { cur_type });
			vec_type->type_kind = Mem::Type::container;
			Mem::type_map.insert({ cur_type,
				 vec_type });
			// set []
			Mem::type_op_type_map.insert({ cur_type, {{"[]",element_type}} });
			// set push_back
			SystemFunction* push_back = new SystemFunction(Mem::BVOID, _vec_push_back);
			push_back->set_param_types({ cur_type,element_type });
			member_function_table[cur_type].insert({ "push_back", push_back });
			// size()
			SystemFunction* size = new SystemFunction(Mem::BVOID, _vec_size);
			size->set_param_types({ cur_type });
			member_function_table[cur_type].insert({ "size", size });
		}

	}
}
