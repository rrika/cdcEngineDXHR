#pragma once
#include "RenderResource.h"

namespace cdc {

class IMaterial;
class IMatrixState;
class RenderMesh;
struct Matrix;
struct NonPersistentPGData;
struct PersistentPGData;

class RenderModelInstance : public RenderResource {
public:
	virtual RenderMesh const *GetRenderMesh() const = 0; // 18
	virtual void setMaterial(uint32_t, IMaterial*) = 0; // 28
	virtual void SetProjectionOverride(Matrix const*) = 0; //
	virtual PersistentPGData *getPersistentPGData() = 0; // 74
	virtual NonPersistentPGData *getNonPersistentPGData() = 0; // 78
	virtual void recordDrawables(IMatrixState*) = 0; // 84
};

}
