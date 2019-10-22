#pragma once
#include <iostream>
#include <string>
namespace mer
{
	int get_line_no();
	class Error
	{
	public:
		Error(const std::string& str) :ems(str) {}
		virtual std::string what()
		{
			const std::string error_msg = "<line " + std::to_string(get_line_no()) + ">" + ems;
			return error_msg + "\n";
		}
	private:
		std::string ems;
	};
	class RuntimeError:public Error
	{
	public:
	private:
	};
}