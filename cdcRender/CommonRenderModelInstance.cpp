#include "CommonRenderModelInstance.h"
#include "PCDX11Material.h" // for cast

namespace cdc {

void CommonRenderModelInstance::SetMaterial(uint32_t selector, IMaterial *material) { // line 180
	// TODO
	ModelData *mesh = renderMesh->getMesh();
	for (uint32_t i=0; i < mesh->primGroupCount; i++) {
		uint32_t primGroup20 = mesh->primGroups[i].dword20;
		if (selector == ~0u || selector == primGroup20 ||
			primGroup20 == 0 && selector == tab0Ext128[i].material->GetId())
			tab0Ext128[i].material = (PCDX11Material*)material;
	}
}

}
