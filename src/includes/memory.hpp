#pragma once
#include <vector>

namespace Mer {
	namespace Mem {
		// default 512 KB
		const size_t default_memory_size = 512 * 1024;

		class Memory {
		public:
			Memory() {
				capacity = default_memory_size;
				buf = new char[default_memory_size];
				bp = 0;
				sp = 0;
				var_idx = 0;
				stac = buf;
			}
			void push_glo(size_t len);
			
			void new_block();
			void end_block();
			size_t push_var(size_t siz);

			void new_func(size_t siz);
			void end_func();
			~Memory() { delete[] buf; }

			// to specify the position of local variables.
			size_t var_idx;
			// to specify the position of global variables.
			size_t glo_var_idx;
			// base ptr 
			size_t bp;
			// cur stack top ptr.
			size_t sp;

			size_t max_index;
			//raw data.
			char* buf;
			char* stac;
		private:
			// before runtime ,check if the memory can alloc all global variables.
			void check_before_runtime();
			// if the memory is not enough to alloc the max block or global variabs, double the capacity.
			void extend(size_t new_length);
			size_t capacity;
			std::vector<int> call_stack;
			std::vector<int> block_stack;
		};
		extern Memory default_mem;
	}
}