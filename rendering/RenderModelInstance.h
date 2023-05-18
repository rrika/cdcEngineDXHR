#pragma once
#include "RenderResource.h"

namespace cdc {

class IMatrixState;
class RenderMesh;
struct NonPersistentPGData;
struct PersistentPGData;

class RenderModelInstance : public RenderResource {
public:
	virtual RenderMesh const *GetRenderMesh() const = 0; // 18
	virtual PersistentPGData *getPersistentPGData() = 0; // 74
	virtual NonPersistentPGData *getNonPersistentPGData() = 0; // 78
	virtual void recordDrawables(IMatrixState*) = 0; // 84
};

}
