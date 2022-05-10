#pragma once
#include <cstdint>

namespace cdc {

class CommonRenderDevice;

class IRenderPassCallback {
public:
	virtual bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) = 0;
	virtual void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) = 0;
};

}
