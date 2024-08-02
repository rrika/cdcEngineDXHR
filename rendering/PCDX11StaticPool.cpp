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
	// m_rangeAlloc(0, 0, 0x7FFFFFFF, bufferSize, 64, pAllocator),
	m_pRenderDevice(pRD)
{
	internalCreateIfDeviceManagerAgrees();
}

PCDX11StaticPool::~PCDX11StaticPool() {
	internalRelease();
}

PCDX11StaticPool::Allocation PCDX11StaticPool::Alloc(uint32_t stride, uint32_t numElements) {
	uint32_t size = stride * numElements;
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = size;
	desc.BindFlags = (D3D11_BIND_FLAG) m_binding;
	ID3D11Buffer *buffer = nullptr;
	deviceManager->getD3DDevice()->CreateBuffer(&desc, nullptr, &buffer);
	return {
		/*m_offset=*/ 0,
		/*m_size=*/ size,
		/*m_pBuffer=*/ buffer
	};
}

void PCDX11StaticPool::Free(Allocation const& alloc) {
	alloc.m_pBuffer->Release();
}

bool PCDX11StaticPool::internalCreate() { return true; };
void PCDX11StaticPool::internalRelease() { /*TODO*/ }

}
