#include <d3d11.h>
#include "cdcSys/Assert.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11Pool.h"

namespace cdc {

static void StaticNotSupported() { FatalError("isStatic must be false"); }

PCDX11Pool::PCDX11Pool(
	bool isStatic,
	uint32_t binding,
	uint32_t bufferSize,
	Allocator *pAllocator)
:
	m_bufferSize(bufferSize),
	m_binding(binding),
	m_alloc(0, 0, 0x7FFFFFFF, bufferSize, 64, pAllocator),
	m_isStatic(isStatic)
{
	internalCreateIfDeviceManagerAgrees();
}

PCDX11Pool::~PCDX11Pool() {
	internalRelease();
}

PCDX11Pool::Allocation PCDX11Pool::Alloc(uint32_t stride, uint32_t numElements) {
	uint32_t size = stride * numElements;
	RangeAllocator::Block *b = m_alloc.Alloc(size, stride/*, 0*/);
	uint32_t bufnum = (b->m_start + b->m_padding) / m_alloc.m_pageSize; // is this an inlined function?
	if (bufnum >= m_buffers.size()) {
		{ ID3D11Buffer *zero = nullptr; m_buffers.resize(bufnum+1, zero); }
		{ unsigned char *zero = nullptr; m_sysMem.resize(bufnum+1, zero); }
		{ bool zero = false; m_needsUpdate.resize(bufnum+1, zero); }
	}
	if (m_buffers[bufnum] == nullptr) {
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = m_bufferSize;
		desc.BindFlags = (D3D11_BIND_FLAG) m_binding;
		if (m_isStatic) {
			StaticNotSupported();
			desc.Usage = D3D11_USAGE_DEFAULT;
		} else {
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = 0x10000; // TODO
		}
		deviceManager->getD3DDevice()->CreateBuffer(&desc, nullptr, &m_buffers[bufnum]);
		m_sysMem[bufnum] = new unsigned char[m_bufferSize]; // VirtualAlloc
		if (m_buffers[bufnum] == nullptr)
			FatalError("CreateBuffer failed");
	}
	return {
		this,
		(b->m_start + b->m_padding) % m_alloc.m_pageSize - m_alloc.m_firstPage,
		size,
		m_buffers[bufnum],
		m_sysMem[bufnum],
		b
	};
}

void PCDX11Pool::Free(Allocation const& alloc) {
	m_alloc.Free(alloc.m_pBlock);
}

void PCDX11Pool::BeginScene() {
	if (m_isStatic)
		StaticNotSupported();
	else
		m_alloc.FreeAll();
}

void PCDX11Pool::EndScene() {
	if (m_isStatic) {
		StaticNotSupported();
		for (bool needsUpdate : m_needsUpdate)
			m_poolNeedsUpdate |= needsUpdate;
	} else {
		UpdateAllBuffers();
	}
}

unsigned char *PCDX11Pool::Lock(Allocation const& alloc) {
	auto block = alloc.m_pBlock;
	if (m_alloc.m_pageSize)
		m_needsUpdate[(block->m_start + block->m_padding) / m_alloc.m_pageSize] = true;
	else
		m_needsUpdate[0] = true;
	return ((unsigned char*)alloc.m_pSysMem) + alloc.m_offset;
}

void PCDX11Pool::Update(Allocation const&) {
	if (m_isStatic && m_poolNeedsUpdate) { // in TR the condition is merely (m_poolNeedsUpdate)
		StaticNotSupported();
		// EnterCriticalSection
		UpdateAllBuffers();
		m_poolNeedsUpdate = false;
		// LeaveCriticalSection
	}
}

void PCDX11Pool::UpdateAllBuffers() {
	ID3D11DeviceContext *deviceContext = deviceManager->getD3DDeviceContext();
	ID3D11Device *device = deviceManager->getD3DDevice();
	for (uint32_t i=0; i<m_buffers.size(); i++) {
		if (true || m_needsUpdate[i]) {
			ID3D11Buffer *buffer = m_buffers[i];
			unsigned char *sysMem = m_sysMem[i];
			D3D11_MAPPED_SUBRESOURCE subresourceData;
			if (m_isStatic) {
				StaticNotSupported();

				// unused
				D3D11_BUFFER_DESC bufferDesc = {};
				bufferDesc.ByteWidth = m_bufferSize;
				bufferDesc.Usage = D3D11_USAGE_STAGING;
				bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				ID3D11Buffer *newBuffer;
				device->CreateBuffer(&bufferDesc, 0, &newBuffer);

				deviceContext->Map(newBuffer, 0, D3D11_MAP_WRITE, 0, &subresourceData);
				memcpy(subresourceData.pData, sysMem, m_bufferSize);
				deviceContext->Unmap(buffer, 0);
				deviceContext->CopyResource(buffer, newBuffer);
				newBuffer->Release();

			} else {
				deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresourceData);
				memcpy(subresourceData.pData, sysMem, m_bufferSize);
				deviceContext->Unmap(buffer, 0);
			}
		}
	}
}

bool PCDX11Pool::internalCreate() { return true; };
void PCDX11Pool::internalRelease() {
	for (auto buffer : m_buffers)
		buffer->Release();
	m_buffers.clear();
}

}
