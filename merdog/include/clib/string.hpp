#pragma once
#include "../structure.hpp"
#include "../function.hpp"
namespace Mer
{
	class StringBase :public StructureBase
	{
	public:
		StringBase();
		FunctionBase * get_function(const std::string &id) override;
		void push_functions(const std::string &str, SystemFunction* func);
		static size_t type_code;
		static std::map<std::string, SystemFunction*>&methods_map();
	private:
		static Mem::Object  _m_substr(std::vector<Mem::Object>& args);
		static Mem::Object  _m_size(std::vector<Mem::Object>& args);
	};
	extern StringBase mstr;
	ParserNode *create_str();
	ParserNode *create_glo_str();
	void using_str();
}