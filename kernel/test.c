#include "test.h"
#include "kprint.h"
#include "memory.h"
#include "paging.h"
#include "panic.h"

void test_memory(uint32_t allocs) {
	uint32_t* array[32];
	uint32_t i, j;

	kprint("Allocations left: %lu\n",allocs);
	if (allocs == 0) return;

	for (i = 0; (i < allocs) && (i < 32); ++i) {
		uint32_t* ptr = (uint32_t*)(kmem_allock() << 12);
		assert(ptr != 0);

		for (j = 0; j < 1024; ++j) {
			ptr[j] = allocs;
		}
		array[i] = ptr;
	}

	test_memory(allocs - 1);

	for (i = 0; (i < allocs) && (i < 32); ++i) {
		for (j = 0; j < 1024; ++j) {
			assert(array[i][j] == allocs);
		}
	}
}

void test_paging(uint32_t kframes) {
	uint32_t* ptr;
	uint32_t count;
	uint32_t n;

	count = 1048576;
	ptr = (uint32_t*)(kframes << 12);

	for (n = 0; n < count; ++n) {
		ptr[n] = n;
	}

	for (n = 0; n < count; ++n) {
		assert(ptr[n] == n);
	}
}