#pragma once
#include <cstdint>
#include "MaterialData.h"

namespace cdc {

class CommonMaterial;
class CommonVertexBuffer;
class TextureMap;
struct RenderTerrainDataHeader;
struct VertexDecl;

struct RenderTerrainData { // 33
	template <typename T>
	struct DataList {
		uint32_t length;
		T data[1];
	};

	uint32_t dword0;
	RenderTerrainDataHeader *pHeader; // 4
	RenderTerrainData::DataList<TextureMap*> *pTextures; // 8
	RenderTerrainData::DataList<CommonMaterial*> *pMaterials; // C
};

struct RenderTerrainDataHeader { // 48
	uint32_t m_flags; // 0
	uint32_t nodeOffset; // 4
	uint32_t numNodes; // 8
	uint32_t groupOffset; // C
	uint32_t numGroups; // 10
	uint32_t layoutOffset; // 14, vertexDeclOffset
	uint32_t numLayouts; // 18, numVertexDecls
	uint32_t dword1C; // 1C, totVertexDeclSize
	uint32_t vbOffset; // 20
	uint32_t numVertexBuffers; // 24
	uint32_t geomOffset; // 28
	uint32_t dword2C; // 2C, totGeomSize
	uint32_t dword30; // 30, numTextures
	uint32_t indicesOffset; // 34, indexBufferOffset
	uint32_t numIndices; // 38
	uint32_t dword3C; // 3C, lodDataOffset, lightMapDataOffset, preTessellationInfoOffset
	uint32_t dword40; // 40, numLODs, numLightMapData, m_resourceNameOffset, m_resourceName
};

struct RenderTerrainGroup { // 89
	union {
		CommonMaterial *m_pMaterial; // 0
		uint32_t materialIndex; // 0
	};
	uint32_t vbIndex; // 4
	uint16_t flags; // 8 (2=shadow only, 4=no shadows)
	uint32_t renderPasses; // C
	union {
		uint32_t m_instanceTextureIndices[4]; // 10
		TextureMap *m_pInstanceTextures[4]; // 10
	};
};

struct SpuRenderTerrainInstanceData { // 154
	MaterialInstanceData matData;
};

struct RenderTerrainInstanceData : public SpuRenderTerrainInstanceData { // 159
	uint32_t dword68;
	uint32_t dword6C;
	uint16_t word70;
	uint16_t word72;
	CommonMaterial *material; // 74
	uint32_t dword78;
	uint32_t dword7C;
};

struct SpuTerrainChunk { // 167
	uint16_t m_groupIndex; // 0
	uint16_t m_numIndices; // 2
	uint32_t m_startIndex; // 4
	// uint32_t m_lightMapDataIndex;
};

struct TerrainChunk : public SpuTerrainChunk { // 175
	uint16_t m_minVertIndex; // 8
	uint16_t m_maxVertIndex; // A
	uint16_t m_lodDataIndex; // C
};

struct TerrainChunkArray { // 185
	uint16_t m_numTerrainChunks;
	TerrainChunk m_chunks[1];
};


struct RenderTerrainVertexBuffer { // 194
	union {
		CommonVertexBuffer *pPrimaryVertexBuffer;
		uint32_t primaryVertexDataOffset;
	};
	uint32_t secondaryVertexDataOffset;
	uint32_t numVertices;
	union {
		VertexDecl *pVertexDecl;
		uint32_t vertexDeclIndex;
	};
};

}
