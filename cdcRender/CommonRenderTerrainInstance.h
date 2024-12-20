#pragma once
#include "IRenderTerrainInstance.h"
#include "RenderExternalResource.h"

namespace cdc {

class CommonRenderTerrain;
class Vector4;
struct RenderTerrainInstanceData;

class CommonRenderTerrainInstance :
	public IRenderTerrainInstance,
	public RenderExternalResource
{
public:
	CommonRenderTerrain *renderTerrain; // C
	RenderTerrainInstanceData *instanceData; // 10
	Vector4 *m_pCurrentInstanceParams = 0; // 18
	// Vector4 *m_pInstanceParams = 0; // 1C
	bool m_isDrawBehindAll = false; // 24, is part of sky

public:
	CommonRenderTerrainInstance(CommonRenderTerrain *renderTerrain);

	void resFree() override { /*TODO*/ }
	void resFill(void* src, uint32_t size, uint32_t offset) override { /*TODO*/ }
	char *resGetBuffer() override { /*TODO*/ return nullptr; }
	void resConstruct() override { /*TODO*/ }
};

}
