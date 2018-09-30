#include "../include/clib/container.hpp"
#include "../include/namespace.hpp"
#include "../include/word_record.hpp"
namespace Mer
{
	using namespace Mem;
	VectorObject::VectorObject(size_t type, int size) :obj_type(type), content(size) {}
	VectorObject::VectorObject(size_t type, const std::vector<Object>& vec) : obj_type(type), content(vec) {}
	Object VectorObject::operator[](Object v)
	{
		// [] don't check v's type or its number;
		// if you need to check index, please replace it with <at(args)>;
		return content[std::static_pointer_cast<Int>(v)->get_value()];
	}
	void using_vector()
	{
		
		Mer::tsymbol_table->push_glo("vector", new ContainerTypeRecorder("vector"));
		type_map.insert({ type_counter ,new Mer::Mem::Type("vector",BasicType(type_counter),std::set<size_t>()) });
	}
	FunctionBase * mVector::get_function(const std ::string &id)
	{
		auto finder = metholds_map.find(id);
		if (finder == metholds_map.end())
		{
			throw Error("method<" + id + ">no found");
		}
		return finder->second;
	}
}