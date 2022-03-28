#pragma once
#include <stdint.h>
#include "Allocator.h"

namespace cdc {

class GameHeapAllocator : public Allocator {
public:
	~GameHeapAllocator();
	void method_04() override;
	void method_08() override;
	void method_0C() override;
	void method_10() override;
	void method_14() override;
	bool isResponsibleForPtr(void*) override;
	void method_1C() override;
	void method_20() override;
	void method_24() override;
	void method_28() override;
	void method_2C() override;
	void method_30() override;
	void *alloc(uint32_t, uint32_t) override;
	void free(void*) override;

	virtual void method_3C();
	virtual void method_40();
	virtual void method_44();
	virtual void method_48();
	virtual void method_4C();
	virtual void method_50();
	virtual void method_54();
	virtual void method_58();
	virtual void method_5C();
	virtual void *alignedAlloc(uint32_t, uint32_t, uint32_t);
	virtual void method_64();
	virtual void method_68();
};

}
