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
				constexpr type_code_index real_tag = (type_code_index)BasicTypeTag::REAL;
				constexpr type_code_index byte_tag = (type_code_index)BasicTypeTag::BYTE;
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


					// real
					{{PLUS, real_tag, real_tag}, {real_tag, add<real_default, real_default, real_default>}},
					{{MINUS, real_tag, real_tag}, {real_tag, sub<real_default, real_default, real_default>}},
					{{MUL, real_tag, real_tag}, {real_tag, mul<real_default, real_default, real_default>}},
					{{DIV, real_tag, real_tag}, {real_tag, div<real_default, real_default, real_default>}},

					// char
					{{PLUS, byte_tag, byte_tag}, {byte_tag, add<byte_default, byte_default, byte_default>}},
					{{MINUS, byte_tag, byte_tag}, {byte_tag, sub<byte_default, byte_default, byte_default>}},
					{{MUL, byte_tag, byte_tag}, {byte_tag, mul<byte_default, byte_default, byte_default>}},
					{{DIV, byte_tag, byte_tag}, {byte_tag, div<byte_default, byte_default, byte_default>}},
					{{MOD, byte_tag, byte_tag}, {byte_tag, mod<byte_default, byte_default, byte_default>}},
					{{AND, byte_tag, byte_tag}, {byte_tag, logical_and<byte_default, byte_default, byte_default>}},
					{{OR, byte_tag, byte_tag}, {byte_tag, logical_or<byte_default, byte_default, byte_default>}},

					{{BAND, byte_tag, byte_tag}, {byte_tag, bitwise_and<byte_default, byte_default, byte_default>}},
					{{BOR, byte_tag, byte_tag}, {byte_tag, bitwise_or<byte_default, byte_default, byte_default>}},
					{{BXOR, byte_tag, byte_tag}, {byte_tag, bitwise_xor<byte_default, byte_default, byte_default>}},
					{{LSH, byte_tag, byte_tag}, {byte_tag, left_shift<byte_default, byte_default, byte_default>}},
					{{RSH, byte_tag, byte_tag}, {byte_tag, right_shift<byte_default, byte_default, byte_default>}},

					{{EQ, byte_tag, byte_tag}, {byte_tag, eq<byte_default, byte_default, byte_default>}},
					{{NE, byte_tag, byte_tag}, {byte_tag, ne<byte_default, byte_default, byte_default>}},
					{{GE, byte_tag, byte_tag}, {byte_tag, ge<byte_default, byte_default, byte_default>}},
					{{LE, byte_tag, byte_tag}, {byte_tag, le<byte_default, byte_default, byte_default>}},
					{{GT, byte_tag, byte_tag}, {byte_tag, gt<byte_default, byte_default, byte_default>}},
					{{LT, byte_tag, byte_tag}, {byte_tag, lt<byte_default, byte_default, byte_default>}},
				};

				auto it = tab.find({ op, left_type, right_type });
				if (it == tab.end())
					return { 0, nullptr };
				return it->second;
			}
			std::pair<type_code_index, type_code_index> op_type_converter(type_code_index t1, type_code_index t2, Tag op)
			{
				const int ari_op = 1, bit_op = 2, logical_op = 3;
				// to store the type of the operation 

				static std::map<Tag, int> tab = {
					{PLUS,ari_op},{MINUS,ari_op},{MUL,ari_op},{DIV,ari_op},{MOD,4},
					{LSH,bit_op},{RSH,bit_op},{BXOR,bit_op},{BAND,bit_op},{BOR,bit_op},
					{AND,logical_op},{OR,logical_op}
				};

				int op_kind = tab[op];

				// if type equals
				if (t1 == t2)
				{
					if (t1 == (int)BasicTypeTag::INT || t2 == (int)BasicTypeTag::BYTE)
						return { t1,t1 };
					if (t1 == (int)BasicTypeTag::REAL)
					{
						if (op_kind != 1 && op_kind != 3)
							return { -1,-1 };
						if (op_kind == logical_op)
							return { (int)BasicTypeTag::INT,(int)BasicTypeTag::INT };
						return { t1,t1 };
					}
				}
				else {
					int flags = 0;
					flags |= 1 << t1;
					flags |= 1 << t2;
					if (flags == ((int)BasicTypeTag::INT | (int)BasicTypeTag::BYTE))
						return { (int)BasicTypeTag::INT,(int)BasicTypeTag::INT };

					if (flags & (int)BasicTypeTag::REAL)
					{
						if (op_kind == ari_op)
							return { (int)BasicTypeTag::REAL,(int)BasicTypeTag::REAL };
						else if (op_kind == logical_op)
							return { (int)BasicTypeTag::INT,(int)BasicTypeTag::INT };
						else
							return { -1,-1 };
					}
				}
				return { -1,-1 };
			}
		}
		namespace UnaryOp {
			uop_type get_cast_op(type_code_index src, type_code_index dest)
			{
				using Info = std::pair<type_code_index, type_code_index>;
				constexpr type_code_index int_tag = (type_code_index)BasicTypeTag::INT;
				constexpr type_code_index real_tag = (type_code_index)BasicTypeTag::REAL;
				constexpr type_code_index char_tag = (type_code_index)BasicTypeTag::BYTE;

				static std::map<Info, uop_type> tab = {
					{{int_tag,real_tag}, cast_type<real_default,int_default>},
					{{int_tag,char_tag}, cast_type<byte_default,int_default>},

					{{real_tag,int_tag}, cast_type<int_default,real_default>},
					{{real_tag,char_tag}, cast_type<byte_default,real_default>},

					{{char_tag,real_tag}, cast_type<real_default,byte_default>},
					{{char_tag,int_tag}, cast_type<int_default,byte_default>},
				};
				auto it = tab.find({ src,dest });
				if (it == tab.end())
					return nullptr;
				return it->second;
			}
			TabRes get_unary_op(Tag op, type_code_index ty)
			{
				using Info = std::pair<Tag, type_code_index>;
				constexpr type_code_index int_tag = (type_code_index)BasicTypeTag::INT;
				constexpr type_code_index real_tag = (type_code_index)BasicTypeTag::REAL;

				static std::map<Info, std::pair<type_code_index, uop_type>> tab = {
					// integer
					{{NOT,int_tag},{int_tag,not_op<int_default,int_default>}},
					{{BNOT,int_tag},{int_tag,bitwise_not<int_default,int_default>}},
					{{MINUS,int_tag},{int_tag,get_neg<int_default,int_default>}},

					{{MINUS,real_tag},{real_tag,get_neg<real_default,real_default>}},
				};

				auto it = tab.find({ op,ty });
				if (it == tab.end())
					return { 0,nullptr };
				return it->second;
			}
		}
	}
}

