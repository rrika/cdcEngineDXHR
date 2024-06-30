#pragma once
#include "IRenderDrawable.h"
#include "PrimitiveContext.h"

namespace cdc {

class PCDX11Material;
class PCDX11RenderDevice;
class PCDX11StreamDecl;
struct MaterialInstanceData;
struct LightReceiverData;

struct PrimitiveInfo { // belongs in .cpp file
	uint32_t m_polyFlags;
	uint32_t m_numVertices;
	uint32_t m_numPrims;
	uint32_t m_d3dPrimType; // D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimType;

	PrimitiveInfo(uint32_t polyFlags, uint32_t numPrims, bool hasSourceIndices);
};

class PCDX11NGAPrimitive : public IRenderDrawable {
public:
	PCDX11RenderDevice *m_pRenderDevice; // C
	PrimitiveContext::State *m_pState; // 10
	const PCDX11Material *m_pMaterial; // 14
	LightReceiverData *m_pLightSet; // const PCDX11LightSet *m_pLightSet; // 18
	float *m_pIrradianceState; // const PCDX11IrradianceState* m_pIrradianceState; // 1C
	MaterialInstanceData *m_pInstanceData; // 20
	uint32_t m_numPrims; // 24
	uint32_t m_numVertices; // 28
	uint32_t m_startIndex; // 2C
	uint32_t topology; // D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimType;

public:
	PCDX11NGAPrimitive(
		PrimitiveContext::State *pState,
		PrimitiveInfo *primInfo,
		uint32_t startIndex,
		float sortKey,
		PCDX11Material *pMaterial,
		PCDX11RenderDevice *pRenderDevice);
	uint32_t BuildInstanceDataAndFlags(MaterialInstanceData& instanceData);

	static bool compare(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void drawDepth(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void drawShadow(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void drawAlphaBloom(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void drawComposite(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void drawTranslucent(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void drawNormal(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);

	void draw(uint32_t funcSetIndex, IRenderDrawable *other) override;
	uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other) override;

	void DrawPrimitive(PCDX11StreamDecl *pStreamDecl, bool isBackBeforeFront, uint32_t flags);
	void SetupTransform(PCDX11NGAPrimitive *prevDrawable);
};

}
