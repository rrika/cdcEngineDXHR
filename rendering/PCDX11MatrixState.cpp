#include "PCDX11MatrixState.h"
#include "PCDX11RenderDevice.h"

namespace cdc {

void PCDX11MatrixState::resize(uint32_t count) {
    poseData = (PoseData*)renderDevice18->linearAlloc30(
        /*size=*/ 16 + 64 * count + 16 * count,
        /*requester=*/ 7);
    // dword10 = renderDevice18->method_3C();
}

}
