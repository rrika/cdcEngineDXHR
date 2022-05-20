#pragma once
#include "RenderMesh.h"
#include "RenderModelInstance.h"

namespace cdc {

class CommonRenderModelInstance : public RenderModelInstance {
protected:
	RenderMesh *renderMesh;
public:
	CommonRenderModelInstance(RenderMesh *renderMesh) :
		renderMesh(renderMesh)
	{}
};

}
