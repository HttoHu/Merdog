#include "../includes/memory.hpp"
namespace Mer {
	namespace Mem {
		Memory default_mem;

		void Memory::new_block()
		{
			block_stack.push_back(var_idx);
		}
		void Memory::end_block()
		{
			var_idx = block_stack.back();
			block_stack.pop_back();
		}
		size_t Memory::push_var(size_t siz)
		{
			auto ret = var_idx;
			var_idx += siz;
			max_index = std::max(max_index, var_idx);
			return ret;
		}

		void Memory::check_before_runtime()
		{
			if (capacity < 2 * (max_index + glo_var_idx))
				extend(2 * (max_index + glo_var_idx));
		}

		void Memory::extend(size_t new_length)
		{
			char * tmp = new char[new_length];
			memcpy(tmp, buf, capacity);
			delete[] buf;
			buf = tmp;
			capacity = new_length;
		}
	}
}

