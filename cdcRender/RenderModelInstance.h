#pragma once
#include "RenderResource.h"

namespace cdc {

class CommonRenderDevice;
class IMaterial;
class IMatrixState;
class RenderMesh;
class TextureMap;
struct Matrix;
struct NonPersistentPGData;
struct PersistentPGData;

class RenderModelInstance : public RenderResource {
public:
	virtual RenderMesh const *GetRenderMesh() const = 0; // 18
	virtual void SetInstanceTexture(uint32_t drawGroupId, uint32_t index, TextureMap *pTextureMap) = 0; // 1C
	virtual void SetMaterial(uint32_t, IMaterial*) = 0; // 28
	virtual void SetDrawBehindAll(bool) = 0; // 44
	virtual void SetProjectionOverride(Matrix const*) = 0; // 48
	virtual PersistentPGData *getPersistentPGData() = 0; // 74
	virtual NonPersistentPGData *getNonPersistentPGData() = 0; // 78
	virtual void recordDrawables(IMatrixState*) = 0; // 84
	virtual CommonRenderDevice *GetRenderDevicePtr() = 0; // 94
};

}
