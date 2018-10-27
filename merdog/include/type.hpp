#pragma once
#include <string>
#include <set>
#include <vector>
#include <map>
namespace Mer
{
	class Token;
	namespace Mem
	{
		class Type;
		enum BasicType
		{
			NDEF = -1, BVOID = 0, INT = 1, DOUBLE = 3, STRING = 5, BOOL = 7
		};
		void set_new_type(const std::string & type_name, Type *t);
		std::string type_to_string(BasicType bt);

		size_t &type_no();
		Type* get_type_by_string(const std::string &str);
		extern std::map<size_t, Type*> type_map;
		extern int type_counter;

		class Type
		{
		public:
			Type(const std::string _name, BasicType bt, const std::set<size_t>& _convertible_types)
				:name(_name), type_code(bt),convertible_types(_convertible_types) {}
			virtual bool compatible(const Type *t);
			virtual bool convertible(const Type *t);
			virtual std::string to_string() const { return "{TYPE:}" + name + "/n"; }
			virtual ~Type() {}



			std::set<size_t> convertible_types;
			BasicType type_code;
			std::string name;
		};
		class CompoundType:public Type
		{
		public:
			CompoundType(Type* _original, const std::vector<Type*>&c) :
				Type(_original->name, _original->type_code, _original->convertible_types), children(c), original(_original){}
			bool compatible(const Type *t)override;
			bool convertible(const Type *t)override;
			std::string to_string()const override
			{
				std::string ret = "{TYPE:}" + name + "<";
				for (const auto &a : children)
				{
					ret += a->to_string();
				}
				return ret;
			}
		private:
			std::vector<Type*> children;
			Type* original;
		};
	}
	namespace Parser
	{
		Mem::Type *parse_type();
	}
}