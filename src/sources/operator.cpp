#include "../includes/operator.hpp"
#include <tuple>

namespace Mer {
	namespace Op
	{
		std::pair<type_code_index, op_type> get_bin_op(Tag op, type_code_index lt, type_code_index rt)
		{
			using Info = std::tuple<Tag, type_code_index, type_code_index>;
			constexpr type_code_index int_tag = (type_code_index)BasicTypeTag::INT;
			static std::map<Info, std::pair<type_code_index, op_type>> tab = {
				// integer
				{{ PLUS,int_tag,int_tag },{int_tag,add<int_default,int_default,int_default>}},
				{{ MINUS,int_tag,int_tag },{int_tag,sub<int_default,int_default,int_default>}},
				{{ MUL,int_tag,int_tag },{int_tag,mul<int_default,int_default,int_default>}},
				{{ DIV,int_tag,int_tag },{int_tag,div<int_default,int_default,int_default>}},
				{{ MOD,int_tag,int_tag},{int_tag,mod<int_default,int_default,int_default>}}
			};

			auto it = tab.find({ op,lt,rt });
			if (it == tab.end())
				return { 0,nullptr };
			return it->second;
		}
	}
}

