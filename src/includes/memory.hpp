#pragma once
namespace Mer {
	namespace Mem {
		// default 512 KB
		const size_t default_memory_size = 512 * 1024;

		class Memory {
		public:

			size_t bp;
			size_t sp;
		private:
			char* buf;
		};
		extern Memory default_mem;
	}
}