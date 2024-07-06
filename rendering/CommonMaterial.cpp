#include "CommonMaterial.h"
#include "MaterialData.h"
#include "Types.h"

namespace cdc {

uint32_t CommonMaterial::GetId() {
	return materialBlob->m_id;
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

uint32_t CommonMaterial::GetBlendMode() {
	return materialBlob->blendStateC;
}

uint32_t CommonMaterial::GetFadeBlendMode() {
	switch (materialBlob->fadeMode) {
	case /*0*/ MaterialBlob::kFadeOff:
		return 0x07010010;
	case /*1*/ MaterialBlob::kFadeAlphaBlend:
		return 0x06540541;
	case /*2*/ MaterialBlob::kFadeAlphaTest:
		return 0x06010010;
	case /*3*/ MaterialBlob::kFadeAlphaToMask:
		return 0x0E010010;
	case /*4*/ MaterialBlob::kFadeAlphaToMaskDither:
		return 0x16010010;
	default:
		return 0x06540541;
	}
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

void CommonMaterial::SetRenderPasses() {
	uint32_t maskA = ~( // ~0x52CB
		(1 << kPassIndexDepth) |
		(1 << kPassIndexComposite) |
		// (1 << kPassIndexOpaque) |
		(1 << kPassIndexTranslucent) |
		// (1 << kPassIndexFullScreenFX) |
		// (1 << kPassIndexPostFSX) |
		(1 << kPassIndexAlphaBloomFSX) |
		(1 << kPassIndexPredator) |
		// (1 << kPassIndex8) |
		(1 << kPassIndexShadow) |
		// (1 << kPassIndexDepthDependent) |
		(1 << kPassIndexNormal) |
		// (1 << kPassIndexDeferredShading) |
		(1 << kPassIndexNonNormalDepth));

	maskA = (1 << kPassIndexDeferredShading); // HACK

	uint32_t maskB = maskA;

	if (materialBlob->dword18 & 2)
		maskB &= ~(1 << kPassIndexAlphaBloomFSX); // ~0x528B

	if (materialBlob->dword1C & 0x40000000)
		maskA &= ~(1 << kPassIndexShadow); // ~0x50CBu

	if (materialBlob->dword18 & 0x400) {
		maskB |= 1 << kPassIndexDepth;
		maskB |= 1 << kPassIndexNonNormalDepth;
	}

	uint32_t maskBlend, maskElse;

	if (materialBlob->dword1C & 2) {
		maskBlend = maskElse = maskB | (1 << kPassIndexPredator);

	} else {
		maskElse = maskA;
		maskElse |= 1 << kPassIndexDepth;
		maskElse |= 1 << kPassIndexComposite;
		if (materialBlob->subMat4C[7]) // has submaterial for normals
			maskElse |= 1 << kPassIndexNormal;
		else
			maskElse |= 1 << kPassIndexNonNormalDepth;

		maskBlend = maskB | (1 << kPassIndexTranslucent);
	}

	uint32_t fadeBlendMode = GetFadeBlendMode();

	// can't figure out the precise correspondence to SetRenderPassMask calls
	m_renderPassesMaskFading = (fadeBlendMode & 1) ? maskBlend : maskElse;
	m_renderPassesMask = (materialBlob->blendStateC & 1) ? maskBlend : maskElse;
}

}
