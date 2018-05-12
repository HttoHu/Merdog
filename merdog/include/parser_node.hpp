#pragma once
#include <string>
#include "var.hpp"
namespace Mer
{
	class ParserNode
	{
	public:
		ParserNode()
		{
			line_no = get_line_no();
		}
		
		virtual ~ParserNode() {}
		virtual std::string to_string()
		{
			return "<empty_node>";
		}
		virtual void record() 
		{
			throw std::runtime_error("called by an indefinite var");
		}
		virtual size_t get_type()
		{
			return 0;
		}
		virtual Mem::Object execute() 
		{
			throw std::runtime_error("called by an indefinite var");
		}
		size_t line_no;
	};
}
