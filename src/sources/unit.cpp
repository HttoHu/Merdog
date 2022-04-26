#include "../includes/unit.hpp"
#include "../includes/defs.hpp"
#include "../includes/binary_op.hpp"

#include "../includes/memory.hpp"

namespace Mer
{
	LConV::LConV(Token* tok) : ParserNode(NodeType::LConV)
	{
		switch (tok->get_tag())
		{
		case TFALSE:
		case TTRUE:
			type = (type_code_index)BasicTypeTag::INT;
			val = new char[sizeof(int_default)];
			*(int_default*)val = tok->get_tag() == TTRUE;
			break;
		case INTEGER:
			type = (type_code_index)BasicTypeTag::INT;
			val = new char[sizeof(int_default)];
			*(int_default*)val = Integer::get_value(tok);
			break;
		case REAL:
			type = (type_code_index)BasicTypeTag::REAL;
			val = new char[sizeof(real_default)];
			*(real_default*)val = Real::get_value(tok);
			break;
		case CHAR_LIT:
			type = (type_code_index)BasicTypeTag::BYTE;
			val = new char[1];
			*val = CharToken::get_value(tok);
			break;
		case STRING:
		{
			std::string tmp = String::get_value(tok);
			type = (type_code_index)BasicTypeTag::STRING;
			val = new char[tmp.size() + 1];
			memcpy(val, tmp.c_str(), tmp.size());
			val[tmp.size()] = '\0';
			len = tmp.size() + 1;
			return;
		}
		default:
			throw Error("syntax error");
		}
		len = type_tab[type]->type_length();
	}
	std::string LConV::to_string() const
	{
		if (type == (int)BasicTypeTag::INT)
			return std::to_string(*(int_default*)val);
		else if (type == (int)BasicTypeTag::REAL)
			return std::to_string(*(real_default*)val);
		else if (type == (int)BasicTypeTag::BYTE)
			return std::string(1, *val);
		else if (type == (int)BasicTypeTag::STRING)
			return val;
		return "unkonwn LConV";
	}
	Print::Print(ParserNode* _node) : ParserNode(NodeType::PRINT), node(_node)
	{
		if (node->get_type() == 0)
			throw Error("print a void val");
	}
	void Print::execute(char* ret)
	{
		node->execute(ret);
		switch ((BasicTypeTag)node->get_type())
		{
			using enum BasicTypeTag;
		case BYTE:
			std::cout << *ret;
			break;
		case INT:
			std::cout << *(int_default*)ret;
			break;
		case REAL:
			std::cout << *(real_default*)ret;
			break;
		case STRING:
			std::cout << ret;
			break;
		default:
			break;
		}
	}
	std::string Print::to_string() const
	{
		return "(print " + node->to_string() + ")";
	}

	Variable::Variable(const std::string& _var_name, type_code_index _type, size_t _pos, size_t* _base_ptr)
		: ParserNode(NodeType::VAR), var_name(_var_name), type(_type), pos(_pos), base_ptr(_base_ptr)
	{
		len = get_type_size(type);
	}
	void Variable::execute(char* ret)
	{
		memcpy(ret, Mem::default_mem.buf + *base_ptr + pos, len);
	}
	char* Variable::get_runtime_pos()
	{
		return Mem::default_mem.buf + *base_ptr + pos;
	}
	std::string Variable::to_string() const
	{
		return "(var " + var_name + ")";
	}

	ArrayIndex::ArrayIndex(size_t _pos, type_code_index _ty, size_t* _base_ptr, const std::vector<size_t>& vec, const std::vector<ParserNode*>& origins) 
		:ParserNode(NodeType::ARRAY_INDEX),type(_ty),pos(_pos),base_ptr(_base_ptr) {
		type_len = get_type_size(type);
		if (vec.size() != origins.size())
			throw Error("invalid array index");
		index = nullptr;

		for (int i = vec.size() - 1; i >= 0; i--)
		{
			if (index == nullptr)
				index = origins.back();
			else {
				auto tmp = new BinOp(BasicToken["*"], index, new LConV(vec[i]));
				index = new BinOp(BasicToken["+"], index, tmp);
			}
		}
	}
	std::string ArrayIndex::to_string() const
	{
		return "(array_index )";
	}
	void ArrayIndex::execute(char* mem)
	{
		index->execute(mem);
		int_default p = *(int_default*)mem * type_len;
		memcpy(mem, Mem::default_mem.buf + *base_ptr, type_len);
	}
}
