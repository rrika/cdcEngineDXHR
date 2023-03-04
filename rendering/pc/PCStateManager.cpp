#include <d3d9.h>
#include "PCStateManager.h"

namespace cdc {

PCStateManager::PCStateManager() {}

void PCStateManager::setIndexBuffer(PCIndexBuffer *indexBuffer) {
	IDirect3DIndexBuffer9 *buffer = nullptr;
	if (indexBuffer)
		buffer = indexBuffer->getD3DBuffer();

	if (buffer != m_indexBufferD3D) {
		m_device->SetIndices(buffer);
		m_indexBufferD3D = buffer;
	}
}

bool PCStateManager::internalCreate() { return true; }; // TODO
void PCStateManager::internalRelease() {}; // TODO

}
