#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
#include "CommonRenderTerrain.h"
#include "PCDX11RenderExternalResource.h"

namespace cdc {

class CommonRenderTerrainInstance;
class PCDX11DeferredLight;
class PCDX11StaticIndexBuffer;
struct LightReceiverData;
struct PCDX11TerrainState;
struct RenderTerrainData;
struct RenderTerrainVertexBuffer;

class PCDX11RenderTerrain :
	public CommonRenderTerrain,
	public PCDX11RenderExternalResource
{
	// inherited:
	// RenderTerrainDataHeader *m_pHeader; // 4
	// RenderTerrainGroup *m_pGroups; // 8
public:
	uint32_t m_resourceDataSize; // 18
	RenderTerrainData *m_pResourceData; // 1C
	IRenderTerrain::Node *m_pNodes; // 20
	TerrainChunkArray **m_ppGeometry; // 24
	void *m_dword28;
public:
	RenderTerrainVertexBuffer *m_pVertexBuffers; // 2C
	PCDX11StaticIndexBuffer *indexBuffer; // 30
private:
	// uint32_t dword34;
public:

	struct RenderChunk {
		TerrainChunk *m_pGeometry;
		union {
			LightReceiverData *m_pLightSet; // PCDX11LightSet
			PCDX11DeferredLight *pShadowLight;
		};
		uint32_t m_passes;
		uint32_t m_sortKey;
	};

	PCDX11RenderTerrain(PCDX11RenderDevice *renderDevice, uint32_t size);
	~PCDX11RenderTerrain();

	void hackDraw(CommonRenderTerrainInstance*, Matrix*);
	void BuildDrawables(PCDX11TerrainState *terrainState);
	void ProcessSinglePassList(
		PCDX11RenderTerrain::RenderChunk *beginChunk,
		PCDX11RenderTerrain::RenderChunk *endChunk,
		PCDX11TerrainState *terrainState,
		uint32_t flags);

	// from RenderResource
	void resFree() override;
	void resFill(void* src, uint32_t size, uint32_t offset) override;
	char *resGetBuffer() override;
	void resConstruct() override;
	// void resMethod10(); // inherited

	// from IRenderTerrain
	Node *GetNodes() override { return m_pNodes; }
	uint32_t NumNodes() override { return m_pHeader->numNodes; }
	// void Render(/*TODO*/) override;
	// void method_24() override;
};

}
