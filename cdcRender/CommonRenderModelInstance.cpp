#include "CommonRenderModelInstance.h"
#include "PCDX11Material.h" // for cast

namespace cdc {

void CommonRenderModelInstance::SetMaterial(uint32_t selector, IMaterial *material) { // line 180
	ModelData *mesh = renderMesh->getMesh();
	if (mesh->vsSelect == -1)
		return;

	SaveInstanceData(/*isModelInstData=*/false);
	for (uint32_t i=0; i < mesh->primGroupCount; i++) {
		// TODO
		uint32_t primGroup20 = mesh->primGroups[i].dword20;
		if (selector == ~0u || selector == primGroup20 ||
			primGroup20 == 0 && selector == tab0Ext128[i].material->GetId())
			tab0Ext128[i].material = (PCDX11Material*)material;
	}
	// ComputeCombinedNodeFlags();
}

void CommonRenderModelInstance::SaveInstanceData(bool isModelInstData) { // line 722
	if (isModelInstData) {
		// allocate a plain RenderModelInstanceData
		if (m_pInstanceData == nullptr) {
			auto *data = new RenderModelInstanceData;
			m_pCurrentInstanceData = data;
			m_pInstanceData = data;
			memset(data, 0, sizeof(RenderModelInstanceData));
			data->projectOverride = identity4x4;
			data->projectOverrideValid = false;
		}

	} else {
		// make a copy of the mesh's PersistentPGData array
		if (m_pPrimGroupInstances == nullptr) {
			ModelData *mesh = renderMesh->getMesh();
			m_pPrimGroupInstances = new PersistentPGData[mesh->primGroupCount];
			memcpy(
				(void*)m_pPrimGroupInstances,
				(void*)tab0Ext128,
				sizeof(PersistentPGData) * mesh->primGroupCount);
			tab0Ext128 = m_pPrimGroupInstances;
		}
	}
	// TODO
}

void CommonRenderModelInstance::FreeInstanceData() { // line 800
	delete m_pInstanceData;
	// TODO
	delete[] m_pPrimGroupInstances;
	// TODO
}

}
