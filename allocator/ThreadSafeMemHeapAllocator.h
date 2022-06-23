#pragma once
#include "GameHeapAllocator.h"

namespace cdc {

class ThreadSafeMemHeapAllocator : public GameHeapAllocator {
public:
	~ThreadSafeMemHeapAllocator() = default;
	bool isResponsibleForPtr(void*) override;
	void method_28() override;
	void method_2C() override;
	void *alloc(uint32_t, AllocRequester) override;
	void free(void*) override;
	void method_3C() override;
	void method_5C() override;
	void *alignedAlloc(uint32_t, uint32_t, uint32_t) override;
	void method_64() override;
};

}
