#pragma once
#include "CommonRenderModelInstance.h"
#include "PCDX11RenderExternalResource.h"
#include "PCDX11RenderModel.h" // for cast from PCDX11RenderModel* to RenderMesh*

namespace cdc {

class PCDX11RenderDevice;
class PCDX11RenderModel;

class PCDX11RenderModelInstance :
	public CommonRenderModelInstance,
	public PCDX11RenderExternalResource
{
	PCDX11RenderModel *renderModel;
public:
	PCDX11RenderModelInstance(PCDX11RenderModel *renderModel, PCDX11RenderDevice *renderDevice) :
		CommonRenderModelInstance(renderModel),
		PCDX11RenderExternalResource(renderDevice)
	{
		baseMask = 1; // hack
	}

	void resRegisterAtRenderDevice() override { /*TODO*/ };
	void resFill(void* src, size_t size, size_t offset) override { /*empty*/ };
	char *resGetBuffer() override { return nullptr; };
	void resConstruct() override { /*empty*/ };
	void resMethod10() override { /*empty*/ };

	void recordDrawables(IMatrixState*) override; // 84

private:
	PCDX11RenderModel *getRenderModel() { return static_cast<PCDX11RenderModel*>(renderMesh); }
};

}
