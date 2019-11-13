/*
		MIT License

		Copyright (c) 2019 HttoHu

		Permission is hereby granted, free of charge, to any person obtaining a copy
		of this software and associated documentation files (the "Software"), to deal
		in the Software without restriction, including without limitation the rights
		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the Software is
		furnished to do so, subject to the following conditions:

		The above copyright notice and this permission notice shall be included in all
		copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
		SOFTWARE.

*/
#pragma once
#include <set>
#include <map>
#include <string>
namespace Mer
{
	class Token;
	namespace Mem
	{
		enum BasicType
		{
			ARRAY = -2, NDEF = -1, BVOID = 0, INT = 1, DOUBLE = 3, STRING = 5, BOOL = 7, CHAR = 9
		};
		class Type
		{
		public:
			enum kind {
				single = 0, container = 1, dictionary = 2
			}type_kind = single;
			Type(const std::string& _name, size_t bt, const std::set<size_t>& _convertible_types)
				:name(_name), type_code(bt), convertible_types(_convertible_types) {}
			bool convertible(const size_t& t);
			void add_compatible_type(size_t type_code);
			virtual std::string to_string() { return  name; }
			virtual ~Type() {}

		protected:
			std::set<size_t> convertible_types;
			size_t type_code;
			std::string name;
		};
		struct ComplexType
		{
			size_t container_type;
			size_t element_type;
			bool operator<(const ComplexType& op)const
			{
				if (container_type == op.container_type)
					return element_type < op.element_type;
				return container_type < op.container_type;

			}
		};
		/*
			* operator_type for instance []
			* for instance string tmp;
			* the type of tmp[3] is char
		*/
		size_t get_type_code();
		size_t get_type_code(Token* tok);
		size_t& type_no();
		// to get a compound type's code like vector<map<int,real>>
		size_t get_ctype_code();
		int regitser_container(size_t container_type, size_t element_type);
		size_t merge(size_t l, size_t r);
		extern std::map<size_t, ComplexType> demerge_table;
		extern std::map<ComplexType, size_t> merge_table;
		extern std::map<size_t, std::map<std::string, size_t>> type_op_type_map;
		extern std::map<std::string, size_t> type_index;
		extern std::map<size_t, Type*> type_map;
		extern int type_counter;
		extern std::map<size_t, void(*)(size_t)> container_register;
		// get the operator function type
		size_t find_op_type(size_t ty, std::string op);
		bool exist_operator(size_t ty, std::string op);
	}
}