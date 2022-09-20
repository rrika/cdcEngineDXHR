#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
#include "IRenderDrawable.h"
#include "TerrainData.h"

namespace cdc {

class CommonRenderTerrainInstance;
class CommonScene;
class PCDX11RenderTerrain;
class PCDX11StreamDecl;

struct PCDX11TerrainState {
	PCDX11RenderTerrain *renderTerrain;
	CommonRenderTerrainInstance *renderTerrainInstance;
	CommonScene *scene; // 8
	// uint32_t m_numLights; // C
	// PCDX11DeferredLight *m_pLights[128]; // 10
	Matrix m_toWorld; // 210
	RenderTerrainInstanceData *m_pInstanceData; // 250
	// Vector *m_pInstanceParams; // 254
	// bool m_shadowOnly; // 258
	// IRenderTerrainInstance::NodeDrawData **m_ppDrawDatas;
	PCDX11TerrainState *nextEntry; // 260
};

struct PCDX11TerrainDrawable : public IRenderDrawable {
public:
	PCDX11RenderTerrain *m_pTerrain; // C
	CommonRenderTerrainInstance *m_pInstance; // 10
	RenderTerrainGroup *m_pGroup; // 14
	RenderTerrainInstanceData *m_pInstData; // 18
	TerrainChunk **m_ppGeoms; // 1C
	uint32_t m_numGeoms; // 20
	uint32_t m_flags; // 24
	float m_opacity; // 28
	Matrix *m_pLocalToWorld; // 2C
	Vector4 *m_pInstanceParams; // 30
	uint32_t m_lightConstantBufferData; // 34 (PCDX11IrradianceState)
	LightReceiverData *m_lightReceiverData; // 38 (PCDX11LightSet)

	PCDX11TerrainDrawable(
		RenderTerrainGroup *pGroup,
		TerrainChunk **ppGeoms,
		uint32_t numGeoms,
		uint32_t flags,
		PCDX11TerrainState *pState);

	static void draw1(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void draw2(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void draw7(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void draw4(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void draw56(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void drawA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);

	static bool compare12(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool compare467(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool compareA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);

	void draw(uint32_t funcSetIndex, IRenderDrawable *other) override;
	uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other) override;

	void draw(
		PCDX11StreamDecl *streamDecl,
		bool renderTwice,
		bool debugColors);
	bool setMatrix(
		IRenderDrawable *prevDrawable);
	void buildAndAssignLightBuffer();
};

}
