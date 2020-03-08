#include <algorithm>
#include "../include/clib/containers.hpp"
#include "../include/namespace.hpp"
#include "../include/word_record.hpp"
#include "../include/compound_box.hpp"
#include "../include/clib/basic_algorithm.hpp"
namespace Mer
{
	bool _conver_to_bool(Mem::Object obj)
	{
		return std::static_pointer_cast<Mem::Bool>(obj)->_value();
	}
	using __member_function = std::function<Mem::Object(const std::vector<Mem::Object>&)>;
	// imitate std::next_permutation 
	/*
	example
		next_permutation(vec,0,vec.size()-1);
	*/
	extern std::vector<Mem::Object> parents_vec;

	void _register_member_function
	(std::string name, type_code_index type, type_code_index ret_type, const std::vector<type_code_index>& param_list, __member_function mf) {
		SystemFunction* tmp = new SystemFunction(ret_type, mf);
		tmp->set_param_types(param_list);
		member_function_table[type].insert({ name,tmp });
	}
	void _register_type_init
	(type_code_index type, const std::vector<type_code_index>& param_list, __member_function mf) {
		SystemFunction* tmp = new SystemFunction(type, mf);
		tmp->set_param_types(param_list);
		type_init_function_map[InitKey(type, param_list)] = tmp;
	}
	//vector
	namespace
	{
		Mem::Object _vec_resize(const std::vector < Mem::Object >& args)
		{
			auto& vec = std::static_pointer_cast<Container::Vector>(parents_vec.back())->content;
			vec.resize(Mem::get_raw<int>(args[0]),vec[0]->clone());
			return nullptr;
		}
		Mem::Object _vec_clear(const std::vector < Mem::Object >& args)
		{
			auto& vec = std::static_pointer_cast<Container::Vector>(parents_vec.back())->content;
			vec.clear();
			return nullptr;
		}
		Mem::Object _vec_insert(const std::vector<Mem::Object>& args)
		{
			auto& vec = std::static_pointer_cast<Container::Vector>(parents_vec.back())->content;
			int startIndex = Mem::get_raw<int>(args[0]);
			vec.insert(vec.begin() + startIndex, args[2]);
			return nullptr;
		}
		// vector<int> vec={1,2,3,4,5,6};
		Mem::Object _init_vec_list(const std::vector<Mem::Object>& args)
		{
			auto list = std::static_pointer_cast<Mem::InitListObj> (args[0]);
			return std::make_shared<Container::Vector>(list->elems);
		}
		Mem::Object _init_vec_n(const std::vector < Mem::Object >& args)
		{
			int count = Mem::get_raw<int>(args[0]);
			return std::make_shared<Container::Vector>(count);
		}
		Mem::Object _init_vec(const std::vector < Mem::Object >& args)
		{
			int count = Mem::get_raw<int>(args[0]);
			return std::make_shared<Container::Vector>(count, args[0]);
		}
		__member_function _vec_next_permuatation(type_code_index type)
		{
			using VecIt = std::vector<Mem::Object>::iterator;


			auto lam = [=](const std::vector<Mem::Object>& args) {
				auto comparator = compare_map[type];
				auto& vec = std::static_pointer_cast<Container::Vector>(args[0])->content;
				int range_from = Mem::get_raw<int>(args[1]);
				int range_to = Mem::get_raw<int>(args[2]);
				return std::make_shared<Mem::Bool>
					(std::next_permutation(vec.begin() + range_from, vec.begin() + range_to, comparator));
			};
			return lam;
		}
		Mem::Object _vec_push_back(const std::vector < Mem::Object >& args)
		{
			auto vec = std::static_pointer_cast<Container::Vector>(parents_vec.back());
			vec->content.push_back(args[0]);
			return nullptr;
		}
		Mem::Object _vec_pop_back(const std::vector < Mem::Object >& args)
		{
			auto vec = std::static_pointer_cast<Container::Vector>(parents_vec.back());
			vec->content.pop_back();
			return nullptr;
		}
		Mem::Object _vec_back(const std::vector < Mem::Object >& args)
		{
			auto vec = std::static_pointer_cast<Container::Vector>(parents_vec.back());
			return vec->content.back();
		}
		Mem::Object _vec_size(const std::vector < Mem::Object >& args)
		{
			auto vec = std::static_pointer_cast<Container::Vector>(parents_vec.back());
			return std::make_shared<Mem::Int>(vec->content.size());
		}
	}
	//deque
	namespace
	{
		Mem::Object _deque_resize(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(parents_vec.back());
			deq->content.resize(Mem::get_raw<int>(args[0]));
			return nullptr;
		}
		Mem::Object _deque_clear(const std::vector < Mem::Object >& args)
		{
			auto& deq = std::static_pointer_cast<Container::Deque>(parents_vec.back())->content;
			deq.clear();
			return nullptr;
		}
		Mem::Object _deque_insert(const std::vector<Mem::Object>& args)
		{
			auto& deq = std::static_pointer_cast<Container::Deque>(parents_vec.back())->content;
			int startIndex = Mem::get_raw<int>(args[0]);
			deq.insert(deq.begin() + startIndex, args[2]);
			return nullptr;
		}
		// vector<int> vec={1,2,3,4,5,6};
		Mem::Object _init_deq_list(const std::vector<Mem::Object>& args)
		{
			auto list = std::static_pointer_cast<Mem::InitListObj> (args[0]);
			return std::make_shared<Container::Deque>(std::deque<Mem::Object>(list->elems.begin(), list->elems.end()));
		}
		Mem::Object _init_deq_n(const std::vector < Mem::Object >& args)
		{
			int count = Mem::get_raw<int>(args[0]);
			return std::make_shared<Container::Deque>(count);
		}
		Mem::Object _init_deq(const std::vector < Mem::Object >& args)
		{
			int count = Mem::get_raw<int>(args[0]);
			return std::make_shared<Container::Deque>(count, args[0]);
		}
		Mem::Object _deq_push_back(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(parents_vec.back());
			deq->content.push_back(args[0]);
			return nullptr;
		}
		Mem::Object _deq_pop_back(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(parents_vec.back());
			deq->content.pop_back();
			return nullptr;
		}
		Mem::Object _deq_push_front(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(parents_vec.back());
			deq->content.push_front(args[0]);
			return nullptr;
		}
		Mem::Object _deq_pop_front(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(parents_vec.back());
			deq->content.pop_front();
			return nullptr;
		}
		Mem::Object _deq_back(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(parents_vec.back());
			return deq->content.back();
		}
		Mem::Object _deq_front(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(parents_vec.back());
			return deq->content.front();
		}
		Mem::Object _deq_size(const std::vector < Mem::Object >& args)
		{
			auto deq = std::static_pointer_cast<Container::Deque>(parents_vec.back());
			return std::make_shared<Mem::Int>(deq->content.size());
		}
		__member_function _deq_next_permuatation(type_code_index type)
		{
			using DeqIt = std::deque<Mem::Object>::iterator;
			auto lam = [=](const std::vector<Mem::Object>& args) {
				auto comparator = compare_map[type];
				auto& deq = std::static_pointer_cast<Container::Deque>(args[0])->content;
				int range_from = Mem::get_raw<int>(args[1]);
				int range_to = Mem::get_raw<int>(args[2]);
				return std::make_shared<Mem::Bool>
					(std::next_permutation(deq.begin() + range_from, deq.begin() + range_to, comparator));
			};
			return lam;
		}
	}
	namespace Container
	{
		int Vector::vector_type_code = 0;
		int Deque::deque_type_code = 0;
		Vector::Vector(int count, Mem::Object obj) :content(count)
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
			this_namespace->sl_table->push("vector", ctr);// register the new-vector-type to the symbol table 
			Mem::container_register.insert({ Vector::vector_type_code,register_new_vector_type });// insert the new type to container_register
			Mem::type_index.insert({ "vector" ,Vector::vector_type_code });// record the type info.
			auto vec_type = new Mem::Type("vector", Vector::vector_type_code, { (type_code_index)Vector::vector_type_code });
			vec_type->type_kind = Mem::Type::kind::container;
			Mem::type_map.insert({ Vector::vector_type_code,
				 vec_type });
		}

		void using_deque()
		{
			// when you create an ContainerTypeRecorder the type_counter will be added two. So you need to sub 2 to
			// get the deque's type;
			auto ctr = new ContainerTypeRecorder("deque");
			Deque::deque_type_code = ctr->get_type();
			this_namespace->sl_table->push("deque", ctr);	// register the new-deque-type to the symbol table 
			Mem::container_register.insert({ Deque::deque_type_code,register_new_deque_type });// insert the new type to container_register
			Mem::type_index.insert({ "deque" ,(size_t)Deque::deque_type_code });  // record the type info 
			auto deque_type = new Mem::Type("deque", Deque::deque_type_code, { (type_code_index)Deque::deque_type_code });
			deque_type->type_kind = Mem::Type::kind::container;
			Mem::type_map.insert({ Deque::deque_type_code,
				 deque_type });
		}
		void register_new_deque_type(type_code_index element_type)
		{
			type_code_index cur_type = Mem::merge(Deque::deque_type_code, element_type);
			// set init============================================================
			_register_type_init(cur_type, { Mem::INT }, _init_deq_n);
			_register_type_init(cur_type, { Mem::INIT_LIST }, _init_deq_list);
			_register_type_init(cur_type, { Mem::INT,element_type }, _init_deq);
			type_init_map[cur_type] = std::make_shared<Deque>();
			// set type============================================================
			auto deq_type = new Mem::Type("deque", cur_type, { cur_type });
			deq_type->type_kind = Mem::Type::kind::container;
			// set next_permutation for deque
			Mem::type_map.insert({ cur_type, deq_type });
			auto deq_next_permutation = new SystemFunction(Mem::BasicType::BVOID, _deq_next_permuatation(element_type));
			deq_next_permutation->set_param_types({ cur_type,Mem::INT,Mem::INT });

			root_namespace->set_new_func("next_permutation", deq_next_permutation);
			// set []====================================================================
			Mem::type_op_type_map.insert({ cur_type, {{"[]",element_type}} });
			// set member functions======================================================
			_register_member_function("push_back", cur_type, Mem::BVOID, { element_type }, _deq_push_back);
			_register_member_function("push_front", cur_type, Mem::BVOID, { element_type }, _deq_push_front);
			_register_member_function("pop_back", cur_type, Mem::BVOID, { }, _deq_pop_back);
			_register_member_function("pop_front", cur_type, Mem::BVOID, {  }, _deq_pop_front);
			_register_member_function("front", cur_type, element_type, {  }, _deq_front);
			_register_member_function("back", cur_type, element_type, {  }, _deq_back);
			_register_member_function("size", cur_type, Mem::INT, { }, _deq_size);
			_register_member_function("resize", cur_type, Mem::BVOID, { Mem::INT }, _deque_resize);
			_register_member_function("clear", cur_type, Mem::BVOID, {}, _deque_clear);
			_register_member_function("insert", cur_type, Mem::BVOID, { Mem::INT,element_type }, _deque_insert);
		}
		void register_new_vector_type(type_code_index element_type)
		{
			type_code_index cur_type = Mem::merge(Vector::vector_type_code, element_type);
			// set init=============================================================
			_register_type_init(cur_type, { Mem::INT }, _init_vec_n);
			_register_type_init(cur_type, { Mem::INIT_LIST }, _init_vec_list);
			_register_type_init(cur_type, { Mem::INT,element_type }, _init_vec);
			type_init_map[cur_type] = std::make_shared<Vector>();
			// set type===========================================================
			auto vec_type = new Mem::Type("vector", cur_type, { cur_type });
			vec_type->type_kind = Mem::Type::kind::container;
			Mem::type_map.insert({ cur_type, vec_type });
			// next_permutation for vector
			auto vec_next_permutation = new SystemFunction(Mem::BasicType::BVOID, _vec_next_permuatation(element_type));
			vec_next_permutation->set_param_types({ cur_type,Mem::INT,Mem::INT });
			root_namespace->set_new_func("next_permutation", vec_next_permutation);
			// set []
			Mem::type_op_type_map.insert({ cur_type, {{"[]",element_type}} });
			// set member functions==================================================
			_register_member_function("push_back", cur_type, Mem::BVOID, { element_type }, _vec_push_back);
			_register_member_function("pop_back", cur_type, Mem::BVOID, { }, _vec_pop_back);
			_register_member_function("back", cur_type, element_type, {  }, _vec_back);
			_register_member_function("clear", cur_type, Mem::BVOID, {}, _vec_clear);
			_register_member_function("size", cur_type, Mem::INT, {  }, _vec_size);
			_register_member_function("resize", cur_type, Mem::BVOID, { Mem::INT }, _vec_resize);
			_register_member_function("insert", cur_type, Mem::BVOID, { Mem::INT,element_type }, _vec_insert);
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
