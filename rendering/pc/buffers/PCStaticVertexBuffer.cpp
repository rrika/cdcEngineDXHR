#include <cstring>
#include <d3d9.h>
#include "PCStaticVertexBuffer.h"
#include "../PCDeviceManager.h"

namespace cdc {

void PCStaticVertexBuffer::Create(void *data, uint32_t numVertices, uint32_t stride) {
	this->stride = stride;
	auto *d3dDevice9 = deviceManager9->getD3DDevice();
	d3dDevice9->CreateVertexBuffer(numVertices * stride,
		0,
		0,
		D3DPOOL_MANAGED,
		&buffer,
		nullptr);
	void *mapped;
	buffer->Lock(0, 0, (void**)&mapped, 0);
	memcpy(mapped, data, numVertices * stride);
	buffer->Unlock();
}

}
