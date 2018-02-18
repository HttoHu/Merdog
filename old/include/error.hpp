#pragma once
#include <string>
#include <exception>
namespace Merdog
{
  class Error:public std::exception
  {
  public:
	  Error(const std::string &str) :ems(str) {}
	  virtual char const* what() const override
	  {
		  return ems.c_str();
	  }
  private:
	  std::string ems;
  };
}
