#include <d3d9.h>
#include "PCDeviceManager.h"
#include "PCStreamDecl.h"

namespace cdc {

void PCStreamDecl::apply() {
	auto *device = deviceManager9->getD3DDevice();
	if (vertexDecl)
		device->SetVertexDeclaration(vertexDecl);
	// TODO
}

bool PCStreamDecl::internalCreate() {
	auto *device = deviceManager9->getD3DDevice();
	if (device->CreateVertexDeclaration(elements, &vertexDecl) >= 0)
		return true;
	return false;
}

void PCStreamDecl::internalRelease() {
	// TODO
}

}
