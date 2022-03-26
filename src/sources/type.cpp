#include "../includes/type.hpp"
#include "../includes/defs.hpp"

namespace Mer {
	
	std::vector<Type*> type_tab = {
		new BasicType("void",0,{}),// 0 void,
		new BasicType("int",sizeof(int_default),{1,2,3}), // 1 int,
		new BasicType("real",sizeof(real_default),{1,2,3}), //2 real,
		new BasicType("byte",sizeof(byte_default),{1,2,3}), // 3 byte
	};
}