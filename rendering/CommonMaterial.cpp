#include "CommonMaterial.h"
#include "MaterialData.h"

namespace cdc {

MaterialBlob *CommonMaterial::GetMaterialData() {
	return materialBlob;
}

void CommonMaterial::SetRenderPassMask(uint32_t mask, bool fading) {
	if (materialBlob->blendStateC & 1) {
		if (fading)
			m_renderPassesMaskFading = mask;
		else
			m_renderPassesMask = mask;
	} else {
		m_renderPassesMaskFading = mask;
		m_renderPassesMask = mask;
	}
}

uint32_t CommonMaterial::GetRenderPassMask(bool fading) {
	return fading ? m_renderPassesMaskFading : m_renderPassesMask;
}

void CommonMaterial::method_18() {
	// TODO
}

void CommonMaterial::SetRenderPasses() { // HACK
	uint32_t mask;

	if (materialBlob->blendStateC & 1)
		mask = 0x0008; // translucent
	else
		mask = 0x1002;  // normals & composite

	m_renderPassesMaskFading = m_renderPassesMask = mask;
}

}
