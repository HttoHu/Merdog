#include "../include/clib/containers.hpp"
#include "../include/Namespace.hpp"
#include "../include/word_record.hpp"
namespace Mer
{
	namespace
	{
		Mem::Object _init_vec(const std::vector < Mem::Object > & args)
		{
			int count = Mem::get_raw<int>(args[0]);
			return std::make_shared<Container::Vector>(count, args[1]);
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

		void using_vecotr()
		{
			// when you create an ContainerTypeRecorder the type_counter will be added two. So you need to sub 2 to
			// get the vector's type;
			auto ctr = new ContainerTypeRecorder("vector");
			Vector::vector_type_code = ctr->get_type() - 2;
			root_namespace->sl_table->push("vector",ctr);
			
		}

		void register_new_vector_type(size_t element_type)
		{
			SystemFunction* init = new SystemFunction(element_type, _init_vec);
			init->set_param_types({ Mem::INT, element_type });
			size_t cur_type = Mem::merge(Vector::vector_type_code, element_type);
			type_init_function_map[cur_type] = init;
			Mem::type_op_type_map.insert({ cur_type, {{"[]",element_type}} });

		}

	}
}
