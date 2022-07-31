#pragma once
#include "RenderResource.h"

namespace cdc {

class IMatrixState;

class RenderModelInstance : public RenderResource {
public:
	// getPersistentPGData // 74
	// getNonPersistentPGData // 78
	virtual void recordDrawables(IMatrixState*) = 0; // 84
};

}
