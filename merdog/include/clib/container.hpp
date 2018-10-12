#pragma once
#include "../structure.hpp"
#include "sys_method.hpp"
#include <vector>
#include <memory>
namespace Mer
{
	class ContainerBase :public Mem::Value
	{
	public:
		ContainerBase(size_t t);
		virtual Mem::Object operator+=(Mem::Object v)override;
		virtual Mem::Object operator[](Mem::Object v)override;
		virtual ~ContainerBase() {}
		size_t obj_type;
	};
	class mVector;
	class MerVecObj :public ContainerBase
	{
	public:
		MerVecObj(size_t t) :ContainerBase(t) {}
		MerVecObj(size_t type, const std::vector<Expr*>& cn);
		virtual Mem::Object operator+=(Mem::Object v)override;
		virtual Mem::Object operator[](Mem::Object v)override;
		virtual ~MerVecObj() {}
	private:
		friend class Mer::mVector;
		std::vector<Mem::Object> content;
	};
	class mVector:public StructureBase
	{
	public:
		mVector();
		FunctionBase * get_function(const std::string &id) override;
		void push_functions(const std::string &str, SMethod* func)
		{
			methods_map().insert({ str, func });
		}
		static size_t type_code;
		static std::map<std::string, SMethod*>&methods_map();
	private:
		Mem::Object  _m_push_back(const std::vector<Mem::Object> &args);
	};
	extern mVector m_vector;


	struct ContainerInit
	{
	public:	
		ContainerInit() {}
		ContainerInit(size_t type) :element_type(type) {}
		Mem::Object make_container();
		std::vector<Expr*> args;
		size_t container_type=0;
		size_t element_type;
	};
	using UContainerInit = std::unique_ptr<ContainerInit>;
	using SContainerInit = std::shared_ptr<ContainerInit>;
	class ContainerDecl :public ParserNode
	{
	public:
		friend ParserNode * parse_def_glo_container();
		ContainerDecl(size_t ct,size_t et, std::map<Token*, UContainerInit> &&v);
		Mem::Object execute()override;
	private:
		size_t container_type=0;
		size_t element_type=0;
		std::vector<std::pair<std::size_t, UContainerInit >> var_list;
	};
	ParserNode *parse_def_container();
	// now I am trying to use unique to prevent memory from leaking.. (Before, I seldom used unique_ptr.
	UContainerInit parse_container_init(size_t element_type);
	// register vector, you can't use vector before enable it.
	void using_vector();
}