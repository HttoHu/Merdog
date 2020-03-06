//labs
#include "../include/memory.hpp"
#include <iostream>
namespace Mer {
	/*
	Memory -> Object array 
	a=b.clone();
	a=b; b will be set to null;



	*/
	namespace lab {
		// exclusive pointer
		class Object {
		public:
			Object(Mem::Value *raw) :raw_ptr(raw) {}
			Object& operator=(Object &obj) {
				delete raw_ptr;
				raw_ptr = obj.raw_ptr;
				obj.raw_ptr = nullptr;
			}
			// deref
			Object clone() {
				// wait 
				// return raw_ptr->clone();
			}
			Mem::Value *operator->() { return raw_ptr; }
			virtual ~Object() {
				if (raw_ptr)delete raw_ptr;
			}
		private:
			Mem::Value *raw_ptr;
		};
	}
}