#pragma once
#include "RenderResource.h"

namespace cdc {

class IMatrixState;

class RenderModelInstance : public RenderResource {
public:
	virtual void recordDrawables(IMatrixState*) = 0; // 84
};

}
