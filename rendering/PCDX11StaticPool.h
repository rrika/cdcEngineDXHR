#pragma once
#include <cstdint>
// #include <vector>
#include "PCDX11InternalResource.h"
//#include "RangeAllocator.h"

struct ID3D11Buffer;

namespace cdc {

class Allocator;
class PCDX11RenderDevice;

class PCDX11StaticPool : public PCDX11InternalResource {
public:
	struct Allocation { // sizeof(Allocation) == 16
		uint32_t m_offset; // 0
		uint32_t m_size; // 4
		ID3D11Buffer *m_pBuffer; // 8
		// RangeAllocator::Block *m_pBlock; // C
	};

	uint32_t m_bufferSize; // C
	uint32_t m_binding; // 10, D3D11_BIND_FLAG
	// std::vector<ID3D11Buffer *> m_buffers; // 14
	// RangeAllocator m_rangeAlloc; // 20
	PCDX11RenderDevice *m_pRenderDevice;

	PCDX11StaticPool(
		PCDX11RenderDevice *pRD,
		uint32_t binding,
		uint32_t bufferSize,
		Allocator *pAllocator);
	~PCDX11StaticPool();

	Allocation Alloc(uint32_t, uint32_t);
	void Free(Allocation const&);

	bool internalCreate() override;
	void internalRelease() override;
};

}
