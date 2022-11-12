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

void CommonMaterial::InitInstanceData(MaterialInstanceData *instData, TextureMap **tex) {
	*instData = MaterialInstanceData();
	for (uint32_t i=0; i<4; i++) {
		// TODO: MaterialInstanceData::setTexture
		instData->pInstanceTextures[i] = tex[i];
		if (tex[i])
			instData->m_updateTextureMask |= 1 << i;
		else
			instData->m_updateTextureMask &= ~(1 << i);
	}
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
