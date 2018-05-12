/*
*		Inspired by
*		https://ruslanspivak.com/lsbasi-part10/
*		Ruslan's Blog
*		C++ Version.
*		Yuantao Hu 2018
*		Email :Huyuantao@outlook.com
*/
#pragma once
#include <exception>
#include <string>
#include <iostream>
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