#include "../includes/operator.hpp"
#include <tuple>

namespace Mer
{
	namespace Op {
		namespace BinOp
		{
			std::pair<type_code_index, bop_type> get_bin_op(Tag op, type_code_index left_type, type_code_index right_type)
			{
				using Info = std::tuple<Tag, type_code_index, type_code_index>;
				constexpr type_code_index int_tag = (type_code_index)BasicTypeTag::INT;
				static std::map<Info, std::pair<type_code_index, bop_type>> tab = {
					// integer
					{{PLUS, int_tag, int_tag}, {int_tag, add<int_default, int_default, int_default>}},
					{{MINUS, int_tag, int_tag}, {int_tag, sub<int_default, int_default, int_default>}},
					{{MUL, int_tag, int_tag}, {int_tag, mul<int_default, int_default, int_default>}},
					{{DIV, int_tag, int_tag}, {int_tag, div<int_default, int_default, int_default>}},
					{{MOD, int_tag, int_tag}, {int_tag, mod<int_default, int_default, int_default>}},
					{{AND, int_tag, int_tag}, {int_tag, logical_and<int_default, int_default, int_default>}},
					{{OR, int_tag, int_tag}, {int_tag, logical_or<int_default, int_default, int_default>}},

					{{BAND, int_tag, int_tag}, {int_tag, bitwise_and<int_default, int_default, int_default>}},
					{{BOR, int_tag, int_tag}, {int_tag, bitwise_or<int_default, int_default, int_default>}},
					{{BXOR, int_tag, int_tag}, {int_tag, bitwise_xor<int_default, int_default, int_default>}},
					{{LSH, int_tag, int_tag}, {int_tag, left_shift<int_default, int_default, int_default>}},
					{{RSH, int_tag, int_tag}, {int_tag, right_shift<int_default, int_default, int_default>}},

					{{EQ, int_tag, int_tag}, {int_tag, eq<int_default, int_default, int_default>}},
					{{NE, int_tag, int_tag}, {int_tag, ne<int_default, int_default, int_default>}},
					{{GE, int_tag, int_tag}, {int_tag, ge<int_default, int_default, int_default>}},
					{{LE, int_tag, int_tag}, {int_tag, le<int_default, int_default, int_default>}},
					{{GT, int_tag, int_tag}, {int_tag, gt<int_default, int_default, int_default>}},
					{{LT, int_tag, int_tag}, {int_tag, lt<int_default, int_default, int_default>}},
				};

				auto it = tab.find({ op, left_type, right_type });
				if (it == tab.end())
					return { 0, nullptr };
				return it->second;
			}
		}
		namespace UnaryOp {
			std::pair<type_code_index, uop_type> get_unary_op(Tag op, type_code_index ty)
			{
				using Info = std::pair<Tag, type_code_index>;
				constexpr type_code_index int_tag = (type_code_index)BasicTypeTag::INT;
				static std::map<Info, std::pair<type_code_index, uop_type>> tab = {
					{{NOT,int_tag},{int_tag,not_op<int_default,int_default>}},
					{{BNOT,int_tag},{int_tag,bitwise_not<int_default,int_default>}},
					{{MINUS,int_tag},{int_tag,get_neg<int_default,int_default>}}
				};
				auto it = tab.find({ op,ty });
				if (it == tab.end())
					return { 0,nullptr };
				return it->second;
			}
		}
	}
}

