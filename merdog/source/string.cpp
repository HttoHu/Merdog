#include "../include/clib/string.hpp"
#include "../include/object.hpp"
#include "../include/value.hpp"
#include "../include/word_record.hpp"
#include "../include/namespace.hpp"
using namespace Mer;

StringBase Mer::mstr;
size_t StringBase::type_code;
Mer::StringBase::StringBase()
{
	SystemFunction *msubstr = new SystemFunction(Mem::BasicType::BVOID, _m_substr);
	msubstr->set_param_types({ Mem::BasicType::INT,Mem::BasicType::INT });
	SystemFunction *msize = new SystemFunction(Mem::BasicType::BVOID, _m_size);
	methods_map().insert({ "substr",msubstr });
	methods_map().insert({ "size",msize });
}

Mer::FunctionBase * Mer::StringBase::get_function(const std::string & id)
{
	auto finder = methods_map().find(id);
	if (finder == methods_map().end())
	{
		throw Error("method<" + id + ">no found");
	}
	return finder->second;
}

void Mer::StringBase::push_functions(const std::string & str, Mer::SystemFunction * func)
{
	methods_map().insert({ str, func });
}
std::map<std::string, Mer::SystemFunction*>& Mer::StringBase::methods_map()
{
	static std::map<std::string, SystemFunction*> ret;

	return ret;
}

Mer::Mem::Object Mer::StringBase::_m_substr(std::vector<Mem::Object>& args)
{
	if (args.size() == 3)
	{
		auto tmp = std::dynamic_pointer_cast<Mem::String>(args[0]);
		auto off = std::dynamic_pointer_cast<Mem::Int>(args[1]);
		auto size = std::dynamic_pointer_cast<Mem::Int>(args[2]);
		return std::make_shared<Mem::String>(tmp->str.substr(off->get_value(), size->get_value()));
	}
	throw Error("argument size error");
}

Mer::Mem::Object Mer::StringBase::_m_size(std::vector<Mem::Object>& args)
{
	auto obj = args[0];
	size_t size = std::static_pointer_cast<Mem::String>(obj)->str.size();
	return std::make_shared<Mem::Int>(size);
}

void Mer::using_str()
{
	auto container_info = new BuildInClass("mstr");
	structure_seeker.insert({ Mem::type_counter,&mstr });

	container_info->create_var = Mer::Parser::var_decl;
	container_info->create_glo_var = Mer::Parser::var_decl;;
	Mer::tsymbol_table->push_glo("mstr", container_info);
	StringBase::type_code = Mer::Mem::type_counter;
	Mem::type_map.insert({ Mem::type_counter ,new Mer::Mem::Type("mstr",Mem::BasicType(Mem::type_counter),std::set<size_t>()) });
	StringBase::methods_map();
}
