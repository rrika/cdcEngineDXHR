#include "CommonMaterial.h"
#include "CommonRenderTerrain.h"
#include "CommonRenderTerrainInstance.h"

namespace cdc {

CommonRenderTerrainInstance::CommonRenderTerrainInstance(CommonRenderTerrain *renderTerrain) :
	renderTerrain(renderTerrain)
{
	uint32_t numGroups = renderTerrain->m_pHeader->numGroups;
	instanceData = new RenderTerrainInstanceData[numGroups];
	for (uint32_t i=0; i<numGroups; i++) {
		RenderTerrainGroup &group = renderTerrain->m_pGroups[i];
		instanceData[i].material = group.m_pMaterial;
		group.m_pMaterial->InitInstanceData(
			&instanceData[i].matData,
			group.m_pInstanceTextures);
	}
}

}
