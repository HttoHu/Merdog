#pragma once
#include "Token.hpp"
#include <map>
#include <vector>
namespace Simcc
{
	namespace Lexer
	{
		class TId :public Token
		{
		public:
			static std::vector<std::map<std::string,TId*>> id_table;
			static TId* find_id(const std::string & str);
			static TId* insert_id(const std::string &id_name);
			static void delete_id_table();
			TId(const std::string & str) :Token(Tag::Id),id_name(str){}
			virtual std::string to_string()const override;
			size_t add = 0;
		private:
			std::string id_name;
		};
	}
}