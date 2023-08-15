#include <d3d9.h>
#include "PCDeviceManager.h"
#include "PCStateManager.h"
#include "PCStreamDecl.h"

namespace cdc {

void PCStreamDecl::apply() {
	auto *device = deviceManager9->getD3DDevice();
	if (vertexDecl)
		device->SetVertexDeclaration(vertexDecl);

	// TODO

	PCStateManager *stateManager = deviceManager9->getStateManager();
	stateManager->SetVertexShaderConstantF(254, (float*)&texcoordScales, 1);
	stateManager->SetVertexShaderConstantF(21, (float*)&normalScaleOffset, 1);
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
