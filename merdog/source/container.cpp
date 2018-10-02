#include "../include/clib/container.hpp"
#include "../include/namespace.hpp"
#include "../include/word_record.hpp"
#include <algorithm>
#include "../include/memory.hpp"
namespace Mer
{
	using namespace Mem;
	size_t mVector::type_code = 0;
	ContainerBase::ContainerBase(size_t type) :obj_type(type) {}
	Object ContainerBase::operator[](Object v)
	{
		// [] don't check v's type or its number;
		// if you need to check index, please replace it with <at(args)>;
		return nullptr;
	}
	ParserNode * parse_def_container()
	{
		// get type=========================
		size_t container_type;
		std::string type_name = Id::get_value(token_stream.this_token());
		if (type_name == "vector") {
			container_type = mVector::type_code;
		}
		token_stream.match(ID);
		token_stream.match(LT);
		size_t element_type = get_type_code(token_stream.this_token());
		token_stream.next();
		token_stream.match(GT);
		//==============================
		std::map<Token*, UContainerInit> var_list;
		auto id = token_stream.this_token();
		token_stream.match(ID);
		if (token_stream.this_token()->get_tag() == LPAREN)
		{
			auto exp = parse_container_init(element_type);
			exp->container_type = container_type;
			var_list.insert({ id,std::move(exp) });
		}
		else
		{
			var_list.insert({ id,std::make_unique<ContainerInit>(element_type) });
		}
		while (token_stream.this_token()->get_tag() == COMMA)
		{
			token_stream.match(COMMA);
			auto id = token_stream.this_token();
			token_stream.match(ID);
			if (token_stream.this_token()->get_tag() == LPAREN)
			{
				auto cinit = parse_container_init(element_type);
				cinit->container_type = container_type;
				var_list.insert({ id,std::move(cinit) });
			}
			else
			{
				var_list.insert({ id,std::make_unique<ContainerInit>(container_type) });
			}
		}
		return new ContainerDecl( container_type,element_type, std::move(var_list));
	}
	UContainerInit parse_container_init(size_t element_type)
	{
		auto ret = std::make_unique<ContainerInit>(element_type);
		if (token_stream.this_tag() != LPAREN)
			return ret;
		token_stream.match(LPAREN);
		while (token_stream.this_tag() != RPAREN)
		{
			ret->args.push_back(new Expr());
			if (token_stream.this_tag() == COMMA)
			{
				token_stream.match(COMMA);
			}
		}
		token_stream.match(RPAREN);
		return ret;
	}
	void using_vector()
	{
		auto container_info = new ContainerTypeRecorder("vector");
		container_info->create_var = parse_def_container;
		Mer::tsymbol_table->push_glo("vector",container_info);
		mVector::type_code = type_counter;
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
	MerVecObj::MerVecObj(size_t type,  const std::vector<Expr*>& cn):ContainerBase(type)
	{
		for (const auto &a : cn) {
			content.push_back(a->execute());
		}
	}
	Mem::Object MerVecObj::operator[](Mem::Object v)
	{
		return content[std::static_pointer_cast<Int>(v)->get_value()];
	}
	ContainerDecl::ContainerDecl(size_t ct, size_t et, std::map<Token*, UContainerInit>&& v):
		container_type(ct),element_type(et)
	{
		for (auto && a:v)
		{
			if (Mer::this_namespace->sl_table->find_front(Id::get_value(a.first)) != nullptr)
				throw Error("Symbol " + a.first->to_string() + " redefined");
			auto pos =  stack_memory.push();
			Mer::this_namespace->sl_table->push(Id::get_value(a.first), new VarIdRecorder(container_type, pos));
			var_list.push_back({ pos,std::move(a.second) });
		}
	}
	Mem::Object ContainerDecl::execute()
	{
		for (const auto &a : var_list)
		{
			stack_memory[a.first] = a.second->make_container();
		}
		return nullptr;
	}
	Object ContainerInit::make_container()
	{
		if (container_type == mVector::type_code)
		{
			return std::make_shared<MerVecObj>(element_type, args);
		}
		else
		{
			throw Error("undefined type");
		}
	}
}