#include <cstring>
#include <d3d9.h>
#include "PCStaticIndexBuffer.h"
#include "../PCDeviceManager.h"

namespace cdc {

void PCStaticIndexBuffer::Create(uint16_t *indices, uint32_t numIndices) {
	this->numIndices = numIndices;
	auto *d3dDevice9 = deviceManager9->getD3DDevice();
	d3dDevice9->CreateIndexBuffer(2*numIndices,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&buffer,
		nullptr);
	void *mapped;
	buffer->Lock(0, 0, (void**)&mapped, 0);
	memcpy(mapped, indices, 2*numIndices);
	buffer->Unlock();
}

}
