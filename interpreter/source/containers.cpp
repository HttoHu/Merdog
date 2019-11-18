#include "../include/clib/containers.hpp"
#include "../include/namespace.hpp"
#include "../include/word_record.hpp"
#include "../include/compound_box.hpp"
namespace Mer
{
	using __member_function=std::function<Mem::Object(std::vector<Mem::Object>&)>;
	void _register_member_function
	(std::string name, size_t type, size_t ret_type,const std::vector<size_t>& param_list, __member_function mf) {
		SystemFunction* tmp = new SystemFunction(ret_type, mf);
		tmp->set_param_types(param_list);
		member_function_table[type].insert({ name,tmp });
	}
	void _register_type_init
		(size_t type,  const std::vector<size_t>& param_list, __member_function mf) {
		SystemFunction* tmp = new SystemFunction(type, mf);
		tmp->set_param_types(param_list);
		type_init_function_map[InitKey(type, param_list)] = tmp;
	}
	//vector
	namespace
	{
		Mem::Object _vec_resize(const std::vector < Mem::Object >& args)
		{
			auto vec = std::static_pointer_cast<Container::Vector>(args[0]);
			int count = Mem::get_raw<int>(args[1]);
			vec->content.resize(count);
			return nullptr;
		}

		Mem::Object _vec_insert(const std::vector<Mem::Object>& args)
		{
			auto& vec = std::static_pointer_cast<Container::Vector>(args[0])->content;
			int startIndex = Mem::get_raw<int>(args[1]);
			vec.insert(vec.begin() + startIndex, args[2]);
			return nullptr;
		}
		// vector<int> vec={1,2,3,4,5,6};
		Mem::Object _init_vec_list (const std::vector<Mem::Object>& args)
		{
			auto list = std::static_pointer_cast<Mem::InitListObj> (args[0]);
			return std::make_shared<Container::Vector>(list->elems);
		}
		Mem::Object _init_vec_n(const std::vector < Mem::Object >& args)
		{
			int count = Mem::get_raw<int>(args[0]);
			return std::make_shared<Container::Vector>(count);
		}
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
		Mem::Object _vec_pop_back(const std::vector < Mem::Object >& args)
		{
			auto vec = std::static_pointer_cast<Container::Vector>(args[0]);
			vec->content.pop_back();
			return nullptr;
		}
		Mem::Object _vec_size(const std::vector < Mem::Object >& args)
		{
			auto vec = std::static_pointer_cast<Container::Vector>(args[0]);
			return std::make_shared<Mem::Int>(vec->content.size());
		}
	}
	//deque
	namespace
	{
		Mem::Object _deque_resize(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(args[0]);
			int count = Mem::get_raw<int>(args[1]);
			deq->content.resize(count);
			return nullptr;
		}

		Mem::Object _deque_insert(const std::vector<Mem::Object>& args)
		{
			auto& deq = std::static_pointer_cast<Container::Deque>(args[0])->content;
			int startIndex = Mem::get_raw<int>(args[1]);
			deq.insert(deq.begin() + startIndex, args[2]);
			return nullptr;
		}
		// vector<int> vec={1,2,3,4,5,6};
		Mem::Object _init_deq_list(const std::vector<Mem::Object>& args)
		{
			auto list = std::static_pointer_cast<Mem::InitListObj> (args[0]);
			return std::make_shared<Container::Deque>(std::deque<Mem::Object>(list->elems.begin(),list->elems.end()));
		}
		Mem::Object _init_deq_n(const std::vector < Mem::Object >& args)
		{
			int count = Mem::get_raw<int>(args[0]);
			return std::make_shared<Container::Deque>(count);
		}
		Mem::Object _init_deq(const std::vector < Mem::Object >& args)
		{
			int count = Mem::get_raw<int>(args[0]);
			return std::make_shared<Container::Deque>(count, args[1]);
		}
		Mem::Object _deq_push_back(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(args[0]);
			deq->content.push_back(args[1]);
			return nullptr;
		}
		Mem::Object _deq_pop_back(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(args[0]);
			deq->content.pop_back();
			return nullptr;
		}
		Mem::Object _deq_push_front(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(args[0]);
			deq->content.push_front(args[1]);
			return nullptr;
		}
		Mem::Object _deq_pop_front(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(args[0]);
			deq->content.pop_front();
			return nullptr;
		}
		Mem::Object _deq_size(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(args[0]);
			return std::make_shared<Mem::Int>(deq->content.size());
		}
	}
	namespace Container
	{
		int Vector::vector_type_code=0;
		int Deque::deque_type_code = 0;
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
			Mem::type_index.insert({ "vector" ,(size_t)Vector::vector_type_code });
			auto vec_type = new Mem::Type("vector", Vector::vector_type_code, { (size_t)Vector::vector_type_code });
			vec_type->type_kind = Mem::Type::container;
			Mem::type_map.insert({ Vector::vector_type_code,
				 vec_type });
		}

		void using_deque()
		{
			// when you create an ContainerTypeRecorder the type_counter will be added two. So you need to sub 2 to
// get the deque's type;
			auto ctr = new ContainerTypeRecorder("deque");
			Deque::deque_type_code = ctr->get_type();
			this_namespace->sl_table->push("deque", ctr);
			Mem::container_register.insert({ Deque::deque_type_code,register_new_deque_type });
			Mem::type_index.insert({ "deque" ,(size_t)Deque::deque_type_code });
			auto deque_type = new Mem::Type("deque", Deque::deque_type_code, { (size_t)Deque::deque_type_code });
			deque_type->type_kind = Mem::Type::container;
			Mem::type_map.insert({ Deque::deque_type_code,
				 deque_type });
		}
		void register_new_deque_type(size_t element_type)
		{
			size_t cur_type = Mem::merge(Deque::deque_type_code, element_type);
			// set init
			_register_type_init(cur_type, { Mem::INT }, _init_deq_n);
			_register_type_init(cur_type, { Mem::INIT_LIST }, _init_deq_list);
			_register_type_init(cur_type, { Mem::INT,element_type }, _init_deq);
			type_init_map[cur_type] = std::make_shared<Deque>();
			// set type
			auto deq_type = new Mem::Type("deque", cur_type, { cur_type });
			deq_type->type_kind = Mem::Type::container;
			Mem::type_map.insert({ cur_type, deq_type });
			// set []
			Mem::type_op_type_map.insert({ cur_type, {{"[]",element_type}} });
			// set member functions
			_register_member_function("push_back", cur_type, Mem::BVOID, { cur_type,element_type }, _deq_push_back);
			_register_member_function("push_front", cur_type, Mem::BVOID, { cur_type,element_type }, _deq_push_front);
			_register_member_function("pop_back", cur_type, Mem::BVOID, { cur_type}, _deq_pop_back);
			_register_member_function("pop_front", cur_type, Mem::BVOID, { cur_type }, _deq_pop_front);
			_register_member_function("size", cur_type, Mem::INT, { cur_type }, _deq_size);
			_register_member_function("resize", cur_type, Mem::BVOID, { cur_type,Mem::INT }, _deque_resize);
			_register_member_function("insert", cur_type, Mem::BVOID, { cur_type,Mem::INT,element_type }, _deque_insert);
		}
		void register_new_vector_type(size_t element_type)
		{
			size_t cur_type = Mem::merge(Vector::vector_type_code, element_type);
			// set init
			_register_type_init(cur_type, { Mem::INT }, _init_vec_n);
			_register_type_init(cur_type, { Mem::INIT_LIST }, _init_vec_list);
			_register_type_init(cur_type, { Mem::INT,element_type }, _init_vec);
			type_init_map[cur_type] = std::make_shared<Vector>();
			// set type
			auto vec_type = new Mem::Type("vector", cur_type, { cur_type });
			vec_type->type_kind = Mem::Type::container;
			Mem::type_map.insert({ cur_type, vec_type });
			// set []
			Mem::type_op_type_map.insert({ cur_type, {{"[]",element_type}} });
			// set member functions
			_register_member_function("push_back", cur_type, Mem::BVOID, { cur_type,element_type }, _vec_push_back);
			_register_member_function("pop_back", cur_type, Mem::BVOID, { cur_type }, _vec_pop_back);

			_register_member_function("size",cur_type, Mem::INT,{ cur_type }, _vec_size);
			_register_member_function("resize", cur_type, Mem::BVOID, { cur_type,Mem::INT }, _vec_resize);
			_register_member_function("insert", cur_type, Mem::BVOID, { cur_type,Mem::INT,element_type }, _vec_insert);
		}

		Deque::Deque(int count, Mem::Object obj) :content(count)
		{
			for (auto& a : content)
			{
				a = obj->clone();
			}
		}

		Mem::Object Deque::operator[](Mem::Object index)
		{
			return content[Mem::get_raw<int>(index)];
		}

		Mem::Object Deque::clone() const
		{
			return std::make_shared<Deque>(content);
		}

	}
}
