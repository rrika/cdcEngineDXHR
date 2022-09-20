#pragma once
#include "IRenderTerrainInstance.h"
#include "RenderExternalResource.h"

namespace cdc {

class CommonRenderTerrain;
struct RenderTerrainInstanceData;

class CommonRenderTerrainInstance :
	public IRenderTerrainInstance,
	public RenderExternalResource
{
public:
	CommonRenderTerrain *renderTerrain;
	RenderTerrainInstanceData *instanceData;

public:
	CommonRenderTerrainInstance(CommonRenderTerrain *renderTerrain);

	void resFree() override { /*TODO*/ }
	void resFill(void* src, uint32_t size, uint32_t offset) override { /*TODO*/ }
	char *resGetBuffer() override { /*TODO*/ return nullptr; }
	void resConstruct() override { /*TODO*/ }
};

}
