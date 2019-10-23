#pragma once
#include <iostream>
#include <map>
namespace mer
{
	enum TypeProperty
	{
		Void,
		Pointer,
		Value,
		Array,
		Class,
	};
	class Type;



	extern std::map<std::string, Type*> type_map;
	extern int type_counter;

	class Type
	{
	public:
		Type(TypeProperty _type_property, std::string _type_name,size_t _size) :type_property(_type_property), type_name(_type_name),type_length(_size) {
			type_code = type_counter++;
		}
		virtual std::string name()const {
			return type_name;
		}
		TypeProperty get_property()const {
			return type_property;
		}
		size_t size()const {
			return type_length;
		}
		virtual ~Type() {}
	private:
		size_t type_length;
		int type_code;
		TypeProperty type_property;
		std::string type_name;
	};
	// we don't name the function as get_type , because if we do so, we will be puzzled between ParserNode::get_type() and mer::get_type();
	Type* process_type();
	void create_value_type(std::string name,size_t length);

}