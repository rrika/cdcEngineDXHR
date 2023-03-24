#pragma once
#include "RenderResource.h"

namespace cdc {

class IMatrixState;
class RenderMesh;

class RenderModelInstance : public RenderResource {
public:
	virtual RenderMesh const *GetRenderMesh() const = 0; // 18
	// getPersistentPGData // 74
	// getNonPersistentPGData // 78
	virtual void recordDrawables(IMatrixState*) = 0; // 84
};

}
