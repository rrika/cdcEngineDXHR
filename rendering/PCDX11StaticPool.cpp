#include <d3d11.h>
#include "PCDX11DeviceManager.h"
#include "PCDX11StaticPool.h"

namespace cdc {

PCDX11StaticPool::PCDX11StaticPool(
	PCDX11RenderDevice *pRD,
	uint32_t binding,
	uint32_t bufferSize,
	Allocator *pAllocator)
:
	m_bufferSize(bufferSize),
	m_binding(binding),
	m_rangeAlloc(0, 0, 0x7FFFFFFF, bufferSize, 64, pAllocator),
	m_pRenderDevice(pRD)
{
	internalCreateIfDeviceManagerAgrees();
}

PCDX11StaticPool::~PCDX11StaticPool() {
	internalRelease();
}

PCDX11StaticPool::Allocation PCDX11StaticPool::Alloc(uint32_t stride, uint32_t numElements) {
	uint32_t size = stride * numElements;
	RangeAllocator::Block *b = m_rangeAlloc.Alloc(size, stride/*, 0*/);
	uint32_t bufnum = (b->m_start + b->m_padding) / m_rangeAlloc.m_pageSize; // is this an inlined function?
	if (bufnum >= m_buffers.size()) {
		ID3D11Buffer *zero = nullptr;
		m_buffers.resize(bufnum+1, zero);
	}
	if (m_buffers[bufnum] == nullptr) {
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = m_bufferSize;
		desc.BindFlags = (D3D11_BIND_FLAG) m_binding;
		deviceManager->getD3DDevice()->CreateBuffer(&desc, nullptr, &m_buffers[bufnum]);
	}
	return {
		(b->m_start + b->m_padding) % m_rangeAlloc.m_pageSize - m_rangeAlloc.m_firstPage,
		size,
		m_buffers[bufnum],
		b
	};
}

void PCDX11StaticPool::Free(Allocation const& alloc) {
	m_rangeAlloc.Free(alloc.m_pBlock);
}

bool PCDX11StaticPool::internalCreate() { return true; };
void PCDX11StaticPool::internalRelease() {
	for (auto buffer : m_buffers)
		buffer->Release();
	m_buffers.clear();
}

}
