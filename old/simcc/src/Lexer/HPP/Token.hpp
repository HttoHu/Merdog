#pragma once
// By 胡远韬 Simcc Hu
#include <iostream>
#include <string>
#include <unordered_map>
namespace Simcc
{
	namespace Lexer
	{
		enum Tag
		{
			SInt, SDouble, SLong, SChar, SString, SBool,
			TLiteralInt, TLiteralLong, TLiteralDouble, TLiteralChar, TLiteralString,
			TWhile, TFor, TBreak, TContinue,
			TIf, TSwitch, TElse, TElif, TCase, TGoto,
			/*else if*/
			TClass, TNamespace, TPublic, TPrivate,
			TReturn,
			Symbol,
			Id,
			Assign, Ge, Gt, Le, Lt, Ne, Eq,
			Add, Sub, Mul, Div, PP, MM,
			True, False,
			TTag,
			MemberPoint, Function,
			Place, //::
			And, Or, // && ,||
			Lk, Rk,// ( and )
			BlockBegin, BlockEnd, // { }
			LSB, RSB,// [ ]
			Comma,// ,
			Endl,
			EndStmt,
			System,
		};
		std::unordered_map<std::string, Tag>& keyword_map();
		std::unordered_map<std::string, Tag>& symbol_map();
		class Token
		{
		public:
			Token(Tag t) :tag(t) {}
			virtual std::string to_string()const
			{
				switch (tag)
				{
				case System:
					return "<system>";
				case TBreak:
					return "<break>";
				case TContinue:
					return "<continue>";
				case True:
					return "<true>";
				case False:
					return "<false>";
				case SBool:
					return "<bool>";
				case SInt:
					return "<int32_t>";
				case SLong:
					return "<long>";
				case SString:
					return "<string>";
				case SDouble:
					return "<double>";
				case SChar:
					return "<char>";

				case Function:
					return "<function>";
				case TTag:
					return "<Tag>";
				case PP:
					return "<++>";
				case MM:
					return "<-->";
				case Endl:
					return "<endline>\n";
				case Assign:
					return "< = >";
				case Eq:
					return "< == >";
				case Ne:
					return "< != >";
				case Ge:
					return "< >= >";
				case Gt:
					return "< > >";
				case Le:
					return "< <= >";
				case Lt:
					return "< < >";
				case Simcc::Lexer::TLiteralInt:
					return "<tint>";
				case Simcc::Lexer::TLiteralLong:
					return "<tlong>";
				case Simcc::Lexer::TLiteralDouble:
					return "<tdouble>";
				case Simcc::Lexer::TLiteralChar:
					return "<tchar>";
				case Simcc::Lexer::TLiteralString:
					return "<tstring>";
				case Simcc::Lexer::TWhile:
					return "<while>";
				case Simcc::Lexer::TIf:
					return "<if>";
				case Simcc::Lexer::TFor:
					return "<for>";
				case Simcc::Lexer::TSwitch:
					return "<switch>";
				case Simcc::Lexer::TClass:
					return "<class>";
				case Simcc::Lexer::TNamespace:
					return "<namespace>";
				case Simcc::Lexer::TReturn:
					return "<return>";
				case Simcc::Lexer::Symbol:
					return "<symbol>";
				case Simcc::Lexer::Add:
					return "<+>";
				case Simcc::Lexer::Sub:
					return "<->";
				case Simcc::Lexer::Mul:
					return "<*>";
				case Simcc::Lexer::Div:
					return "</>";
				case Simcc::Lexer::And:
					return "<&&>";
				case Simcc::Lexer::Or:
					return "<||>";
				case Simcc::Lexer::Lk:
					return "<(>";
				case Simcc::Lexer::Rk:
					return "<)>";
				case Simcc::Lexer::BlockBegin:
					return "<{>";
				case Simcc::Lexer::BlockEnd:
					return "<}>";
				case Simcc::Lexer::LSB:
					return "<[>";
				case Simcc::Lexer::RSB:
					return "<]>";
				case Simcc::Lexer::MemberPoint:
					return "<.>";
				case Simcc::Lexer::Comma:
					return "<,>";
				case Simcc::Lexer::EndStmt:
					return "<;>";
				default:
					return "<unknown>";
					break;
				}
			}
			virtual void* get_value()
			{
				return nullptr;
			}
			virtual const void* const get_value()const
			{
				return nullptr;
			}
			Tag get_tag()const { return tag; }
			virtual ~Token() {}
		private:
			Tag tag;
		};
	}
}