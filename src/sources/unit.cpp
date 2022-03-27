#include "../includes/unit.hpp"
#include "../includes/defs.hpp"
namespace Mer {
	LConV::LConV(Token* tok) :ParserNode(NodeType::LConV)
	{
		switch (tok->get_tag())
		{
		case TFALSE:
		case TTRUE:
			type = (type_code_index)BasicTypeTag::BYTE;
			val = new char[1];
			*val = tok->get_tag() == TTRUE;
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
		default:
			throw Error("syntax error");
		}
		len = type_tab[type]->type_length();
	}
	std::string LConV::to_string() const
	{
		if (type == (int)BasicTypeTag::INT)
			return std::to_string(*(int_default*)val);
		return "unkonwn LConV";
	}
}

