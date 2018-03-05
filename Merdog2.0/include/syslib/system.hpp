#include "../function.hpp"
namespace Mer
{
	namespace Sys
	{
		void import_io();
		void import_function(size_t type,const std::function<Mem::Object(std::vector<Mem::Object>&)> &func,std::string name);
		Mer::Mem::Object console_output(std::vector<Mer::Mem::Object>&);
		Mer::Mem::Object console_input(std::vector<Mer::Mem::Object>&);
	}
}