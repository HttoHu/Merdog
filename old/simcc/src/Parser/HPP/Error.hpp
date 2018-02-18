#pragma once
#include <string>

namespace Simcc
{
	namespace Error
	{
		class SyntaxError:public std::exception
		{
		public:
			SyntaxError(const std::string &str):error_content(str) {}
			virtual char const* what() const;
		private:
			std::string error_content;
		};
	}
}