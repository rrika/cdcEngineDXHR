#include "PCDX11ClearDrawable.h"
#include "PCDX11RenderDevice.h"

namespace cdc {

void PCDX11ClearDrawable::renderDrawable0() {
	float clearColorF[4] = {
		float(((clearColor >> 16) & 0xff) / 255.0),
		float(((clearColor >>  8) & 0xff) / 255.0),
		float(((clearColor >>  0) & 0xff) / 255.0),
		float(((clearColor >> 24) & 0xff) / 255.0)
	};
	renderDevice->clearRenderTarget(
		flags,
		clearColorF,
		clearDepth,
		clearStencil);
}

uint32_t PCDX11ClearDrawable::renderDrawable4() {
	return 1;
}

}
