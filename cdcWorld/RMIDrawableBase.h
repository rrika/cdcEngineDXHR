#pragma once
#include "../scene/IDrawable.h"

namespace cdc {

class RenderModelInstance;
class IMatrixState;
class RenderMesh;

}

class RMIDrawableBase : public cdc::IDrawable {
public:
	cdc::RenderModelInstance *rmi = nullptr;
	cdc::IMatrixState *matrixState = nullptr;
public:
	RMIDrawableBase(cdc::RenderMesh *model);
	~RMIDrawableBase();
	
	void GetBoundingVolume(cdc::BasicCullingVolume*) override;
	void draw(cdc::Matrix *, float) override;
};
