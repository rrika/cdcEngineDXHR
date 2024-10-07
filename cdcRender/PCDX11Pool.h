#pragma once
#include <cstdint>
#include <vector>
#include "PCDX11InternalResource.h"
#include "RangeAllocator.h"

struct ID3D11Buffer;

namespace cdc {

class Allocator;

class PCDX11Pool : public PCDX11InternalResource {
public:
	struct Allocation { // sizeof(Allocation) == 0x18
		PCDX11Pool *m_pPool; // 0
		uint32_t m_offset; // 4
		uint32_t m_size; // 8
		ID3D11Buffer *m_buffer; // C
		void *m_pSysMem; // 10
		RangeAllocator::Block *m_pBlock; // 14
	};

	uint32_t m_bufferSize; // C, same size for all buffers
	uint32_t m_binding; // 10, D3D11_BIND_FLAG
	std::vector<ID3D11Buffer *> m_buffers; // 14
	std::vector<unsigned char *> m_sysMem; // 20
	std::vector<bool> m_needsUpdate; // 2C
	RangeAllocator m_alloc; // 38
	bool m_isStatic; // 68
	bool m_poolNeedsUpdate; // 69
	// _RTL_CRITICAL_SECTION m_criticalSection; // 6C


	// PCDX11Pool(const cdc::PCDX11Pool&);
	PCDX11Pool(bool isStatic, uint32_t binding, uint32_t bufferSize, Allocator*);
	~PCDX11Pool();

	Allocation Alloc(uint32_t alignment, uint32_t size);
	void Free(Allocation const&);
	// void FreeAll();
	void BeginScene();
	void EndScene();
	unsigned char *Lock(Allocation const&);
	void Update(Allocation const&);
	// const void* GetSourceData(Allocation const*);
	void UpdateAllBuffers();

	bool internalCreate() override;
	void internalRelease() override;

};

}
