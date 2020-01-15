#include "../include/optimizer.hpp"
#include "../include/expr.hpp"
#include "../include/value.hpp"
namespace Mer
{
	namespace {
		Mem::Object add(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator+(rhs);
		}
		Mem::Object sub(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator-(rhs);
		}
		Mem::Object mul(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator*(rhs);
		}
		Mem::Object div(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator/(rhs);
		}
		Mem::Object sadd(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator+=(rhs);
		}
		Mem::Object ssub(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator-=(rhs);
		}
		Mem::Object smul(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator*=(rhs);
		}
		Mem::Object sdiv(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator/=(rhs);
		}
		Mem::Object assign(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator=(rhs);
		}
		Mem::Object equal(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator==(rhs);
		}
		Mem::Object not_equal(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator!=(rhs);
		}
		Mem::Object gt(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator>(rhs);
		}
		Mem::Object lt(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator<(rhs);
		}
		Mem::Object ge(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator>=(rhs);
		}
		Mem::Object le(const Mem::Object& lhs, const Mem::Object& rhs) {
			return lhs->operator<=(rhs);
		}
	}
	namespace optimizer
	{
		std::map<Mer::Tag, Mem::Object(*) (const Mem::Object&, const Mem::Object&)> op_table{
			{Mer::PLUS,add},{MINUS,sub},{MUL,mul},{DIV,div},{SADD,sadd},{SDIV,sdiv},{SADD,sadd},
			{SSUB,ssub},{ASSIGN,assign},{EQ,equal},{NE,not_equal},{GT,gt},{GE,ge},{LT,lt},{LE,le}
		};
		ParserNode* optimize_bin_op(ParserNode* left, ParserNode* right, Token* tok)
		{
			type_code_index ty = left->get_type();
			if (typeid(*left) == typeid(LConV) && typeid(*right) == typeid(LConV)) {
				Mem::Object left_v = left->execute();
				Mem::Object right_v;
				if (right->get_type() != ty)
					right_v = right->execute()->Convert(ty);
				else
					right_v = right->execute();
				Mem::Object ret;
				switch (tok->get_tag())
				{
				case SADD:
					ret = left_v->operator+=(right_v);
					break;
				case SSUB:
					ret = left_v->operator-=(right_v);
					break;
				case SDIV:
					ret = left_v->operator/=(right_v);
					break;
				case SMUL:
					ret = left_v->operator*=(right_v);
					break;
				case ASSIGN:
					ret = left_v->operator=(right_v);
					break;
				case PLUS:
					ret = left_v->operator+(right_v);
					break;
				case MINUS:
					ret = left_v->operator-(right_v);
					break;
				case MUL:
					ret = left_v->operator*(right_v);
					break;
				case DIV:
					ret = left_v->operator/(right_v);
					break;
				case EQ:
					ret = left_v->operator==(right_v);
					break;
				case NE:
					ret = left_v->operator!=(right_v);
					break;
				case GT:
					ret = left_v->operator>(right_v);
					break;
				case GE:
					ret = left_v->operator>=(right_v);
					break;
				case LT:
					ret = left_v->operator<(right_v);
					break;
				case LE:
					ret = left_v->operator<=(right_v);
					break;
				default:
					throw Error("Undefined operator");
				}
				delete left; delete right;
				return new LConV(ret, ty);
			}
			return new BinOp(left, tok, right);
		}
		ParserNode* optimize_unary_op(ParserNode* left, Token* tok)
		{
			Mem::Object ret;
			if (typeid(*left) == typeid(LConV)) {
				switch (tok->get_tag())
				{
				case NOT:
				case MINUS:
				{
					auto tmp = left->execute();
					ret = tmp->get_negation();
					break;
				}
				case PLUS:
					ret = left->execute();
				default:
					throw Error("no matched operator");
				}
				type_code_index ty = left->get_type();
				delete left;
				return new LConV(ret, ty);
			}
			return new UnaryOp(tok, left);
		}
		ParserNode* optimize_array_subscript(ParserNode* arr, ParserNode* subscript)
		{
			type_code_index type = arr->get_type();
			if (typeid(*arr) == typeid(Variable) && static_cast<Variable*>(arr)->arr())
			{
				int index = 0;
				if (typeid(*subscript) == typeid(LConV))
				{
					auto v = static_cast<LConV*>(subscript);
					// skip arr info
					index = Mem::get_raw<int>(v->execute()) + 1;
					// get element type;
					auto ret = new Variable(type, arr->get_pos() + index);
					delete arr;
					delete subscript;
					return ret;
				}
				else
				{
					auto ret = new ContainerIndex(type, arr->get_pos() + 1, subscript);
					delete arr;
					return ret;
				}
			}
			return new SubScript(arr, subscript);
		}

	}
}