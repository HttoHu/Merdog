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
using type_code_index = int;
#include <set>
#include <map>
#include <string>
namespace Mer
{
	class Token;
	class FunctionBase;
	extern std::map<type_code_index, std::string> type_name_mapping;
	namespace Mem
	{

		enum BasicType
		{
			ARRAY = -2, NDEF = -1, BVOID = 0, INT = 1, DOUBLE = 3, STRING = 5, BOOL = 7, CHAR = 9,
			INIT_LIST=11,
		};
		class Type
		{
		public:
			enum class kind {
				single = 0, container = 1, dictionary = 2,structure=3,
			}type_kind = kind::single;
			Type(const std::string& _name, type_code_index bt, const std::set<type_code_index>& _convertible_types)
				:name(_name), type_code(bt), convertible_types(_convertible_types) {}
			bool convertible(const type_code_index& t);
			void add_compatible_type(type_code_index type_code);
			virtual std::string to_string() { return  name; }
			virtual ~Type() {}

		protected:
			std::set<type_code_index> convertible_types;
			type_code_index type_code;
			std::string name;
		};
		struct ComplexType
		{
			type_code_index container_type;
			type_code_index element_type;
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
		type_code_index get_type_code();
		type_code_index get_type_code(Token* tok);
		type_code_index& type_no();
		// to get a compound type's code like vector<map<int,real>>
		type_code_index get_ctype_code();

		int register_container(type_code_index container_type, type_code_index element_type);
		type_code_index merge(type_code_index l, type_code_index r);
		std::pair<type_code_index, type_code_index>
			demerge(type_code_index t);
		extern std::map<type_code_index, ComplexType> demerge_table;
		extern std::map<ComplexType, type_code_index> merge_table;
		extern std::map<type_code_index, std::map<std::string, type_code_index>> type_op_type_map;
		extern std::map<std::string, type_code_index> type_index;
		extern std::map<type_code_index, Type*> type_map;
		extern type_code_index type_counter;
		extern std::map<type_code_index, void(*)(type_code_index)> container_register;
		// get the operator function type
		type_code_index find_op_type(type_code_index ty, std::string op);
		bool exist_operator(type_code_index ty, std::string op);
	}
}