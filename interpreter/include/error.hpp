#pragma once
#include <exception>
#include <string>
#include <stdio.h>
#include <iostream>
#define DEBUG_TOOLS(msg) printf("FILE: %s, LINE: %d,FUNCTION: %s,CONTENT: %s",\
__FILE__, __LINE__ , __FUNCTION__,msg)
namespace Mer
{
	size_t get_line_no();
	class Error
	{
	public:
		Error(const std::string &str) :ems(str) {}
		std::string what()
		{
			const std::string error_msg = "<line " + std::to_string(get_line_no()) + ">" + ems;
			return error_msg + "\n";
		}
	private:
		std::string ems;
	};
}