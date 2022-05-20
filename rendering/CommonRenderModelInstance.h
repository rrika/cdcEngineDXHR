#pragma once
#include "RenderMesh.h"
#include "RenderModelInstance.h"
#include <cstdint>

namespace cdc {

class CommonRenderModelInstance : public RenderModelInstance {
protected:
	uint32_t baseMask; // 8
	RenderMesh *renderMesh;
	MeshTab0Ext16 *tab0Ext16; // 24
	MeshTab0Ext128 *tab0Ext128; // 2C
public:
	CommonRenderModelInstance(RenderMesh *renderMesh) :
		renderMesh(renderMesh)
	{
		tab0Ext16 = renderMesh->getTab0Ext16();
		tab0Ext128 = renderMesh->getTab0Ext128();
	}
};

}
