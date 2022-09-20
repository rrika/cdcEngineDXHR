#pragma once
#include "IRenderTerrain.h"
#include "TerrainData.h"

namespace cdc {

class CommonRenderTerrain :
	public IRenderTerrain
{
public:
	RenderTerrainDataHeader *m_pHeader; // 4
	RenderTerrainGroup *m_pGroups; // 8
public:
	//
};

}
