#pragma once
#include <exception>
#include <string>
namespace Mer
{
  class Error:public std::exception
  {
  public:
	  Error(const std::string &str) :ems(str) {}
	  virtual const char* what() const noexcept override
	  {
		  return ems.c_str();
	  }
  private:
	  std::string ems;
  };
}