#pragma once
#include "RenderResource.h"

namespace cdc {

class RenderModelInstance : public RenderResource {
public:
	virtual void recordDrawables() = 0; // 84
};

}
