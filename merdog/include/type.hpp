#pragma once
#include <set>
#include <map>
#include <string>
namespace Mer
{
	namespace Mem
	{
		enum BasicType
		{
			NDEF = -1, BVOID = 0, INT = 1, DOUBLE = 3, STRING = 5, BOOL = 7
		};
		class Type
		{
		public:
			Type(const std::string &_name, size_t bt, const std::set<size_t>& _convertible_types)
				:name(_name), type_code(bt), convertible_types(_convertible_types) {}
			bool convertible(const size_t &t);
			void add_compatible_type(size_t type_code);
			virtual std::string to_string() { return "{TYPE:}" + name + "/n"; }
			virtual ~Type() {}

		protected:
			std::set<size_t> convertible_types;
			size_t type_code;
			std::string name;
		};
		class ComplexType :public Type
		{
		public:
			ComplexType(size_t ct, size_t et);
			virtual std::string to_string();
			size_t get_container_type()const { return container_type; }
			size_t get_element_type()const { return element_type; }
		private:
			size_t container_type;
			size_t element_type;
		};
		extern std::map<std::string, size_t> type_index;
		extern std::map<size_t, Type*> type_map;
		extern int type_counter;
	}
}