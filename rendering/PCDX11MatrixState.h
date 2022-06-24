#pragma once
#include "IMatrixState.h"
#include "PCDX11RenderExternalResource.h"

namespace cdc {

class PCDX11RenderDevice;

struct PoseData {
    uint32_t numMatrices;
    uint32_t padding[3];
    float data[];

    inline float *getMatrix(uint32_t index) {
        return &data[16 * index];
    }

    inline float *getVector(uint32_t index) {
        return &data[16 * numMatrices + 4 * index];
    }
};

class PCDX11MatrixState :
    public IMatrixState,
    public PCDX11RenderExternalResource
{
public:
    PoseData *poseData; // C
    // uint32_t dword10;
    PCDX11RenderDevice *renderDevice18; // even though RenderExternalResource already has a copy
    // uint32_t dword18;
public:
    PCDX11MatrixState(PCDX11RenderDevice *renderDevice) :
        IMatrixState(),
        PCDX11RenderExternalResource(renderDevice),
        poseData(nullptr),
        renderDevice18(renderDevice)
    {}

    void resize(uint32_t) override;
};

}
