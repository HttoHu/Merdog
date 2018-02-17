#pragma once
#include <exception>
#include <string>
#include <iostream>
namespace Mer
{
  size_t get_line_no();
  class Error:public std::exception
  {
  public:
	  Error(const std::string &str) :ems(str) {}
	  virtual const char* what() const noexcept override
	  {
		  const std::string error_msg = "<line "+std::to_string(get_line_no())+">"+ems;
		  std::cout << "TEST: <" + error_msg + ">\n";
		  return ems.c_str();
	  }
  private:
	  std::string ems;

  };
}