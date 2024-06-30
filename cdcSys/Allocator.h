#pragma once
#include <stdint.h>

namespace cdc {

enum AllocRequester : uint32_t {
	kAllocMisc,
	kAllocDebug,
	kAllocContainer,
	kAllocList,
	kAllocSList,
	kAllocQueue,
	kAllocStack,
	kAllocArray,
	kAllocSArray,
	kAllocArrayMap,
	kAllocFixedArray,
	kAllocFixedHash,
	kAllocFixedHashMap,
	kAllocFixedHashSet
};

class Allocator {
public:
	virtual ~Allocator() = default;
	virtual void method_04() = 0;
	virtual void method_08() = 0;
	virtual void method_0C() = 0;
	virtual void method_10() = 0;
	virtual void method_14() = 0;
	virtual bool isResponsibleForPtr(void*) = 0;
	virtual void method_1C() = 0;
	virtual void method_20();
	virtual void method_24() = 0;
	virtual void method_28();
	virtual void method_2C();
	virtual void method_30();
	virtual void *alloc(uint32_t, AllocRequester) = 0;
	virtual void free(void*) = 0;
};

}
