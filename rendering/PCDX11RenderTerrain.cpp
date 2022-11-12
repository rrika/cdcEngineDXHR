#include <cstdio>
#include <cstring>
#include <windows.h>
#include "buffers/PCDX11StaticIndexBuffer.h"
#include "buffers/PCDX11StaticVertexBuffer.h"
#include "CommonRenderTerrainInstance.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderTerrain.h"
#include "PCDX11TerrainDrawable.h"
#include "TerrainData.h"
#include "Types.h"
#include "VertexAttribute.h"

namespace cdc {

PCDX11RenderTerrain::PCDX11RenderTerrain(PCDX11RenderDevice *renderDevice, uint32_t size) :
	CommonRenderTerrain(),
	PCDX11RenderExternalResource(renderDevice)
{
	m_resourceDataSize = size;
#ifdef _WIN32
	m_pResourceData = (RenderTerrainData*) VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	m_pResourceData = (RenderTerrainData*) new char[size];
#endif
}

PCDX11RenderTerrain::~PCDX11RenderTerrain() {
	if (m_pResourceData) {
	#ifdef _WIN32
		VirtualFree(m_pResourceData, 0, MEM_RELEASE);
	#else
		delete[] (char*) m_pResourceData;
	#endif
	}
}

void PCDX11RenderTerrain::hackDraw(CommonRenderTerrainInstance *instance, Matrix *matrix) {
	auto *terrainState = new (renderDevice, 0) PCDX11TerrainState;

	terrainState->renderTerrain = this;
	terrainState->renderTerrainInstance = nullptr;
	terrainState->scene = renderDevice->getScene();
	terrainState->m_toWorld = *matrix;
	terrainState->m_pInstanceData = instance->instanceData;
	terrainState->nextEntry = nullptr;

	// instead of queueing the terrainstate on the renderdevice
	// build the drawables right now
	BuildDrawables(terrainState);
}

static PCDX11RenderTerrain::RenderChunk s_mergableChunks[0x10000];

void PCDX11RenderTerrain::BuildDrawables(PCDX11TerrainState *terrainState) {

	uint32_t flags = 0; // TODO

	{
		Matrix& m = terrainState->m_toWorld;
		float *a = m.m[0];
		float *b = m.m[1];
		float *c = m.m[2];
		float cross[] = {
			a[1] * b[2] - a[2] * b[1],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - a[1] * b[0]
		};
		float sign = cross[0] * c[0] + cross[1] * c[1] + cross[2] * c[2];
		// sign *= terrainState->scene.viewport.invViewMatrix.m[0][0];
		sign *= -1.0f; // HACK
		if (sign < 0.0f)
			flags |= 2;
	}

	// hack implementation
	for (uint32_t i=0; i<m_pHeader->numNodes; i++) {
		TerrainChunkArray *chunkArray = m_ppGeometry[i];
		if (!chunkArray)
			continue;

		for (uint32_t j=0; j<chunkArray->m_numTerrainChunks; j++) {
			TerrainChunk *chunk = &chunkArray->m_chunks[j];
			RenderTerrainGroup *group = &m_pGroups[chunk->m_groupIndex];
			auto **terrainChunkPtr = new (renderDevice->linear34) TerrainChunk*;
			*terrainChunkPtr = chunk;
			auto *drawable = new (renderDevice, 0) PCDX11TerrainDrawable(
				group,
				terrainChunkPtr,
				1,
				flags,
				terrainState);

			renderDevice->recordDrawable(drawable, group->renderPasses, false);
		}
	}

/*
	PCDX11RenderTerrain::RenderChunk *chunkIt = s_mergableChunks;	
	ProcessSinglePassList(s_mergableChunks, chunkIt, terrainState, flags);
*/
}

/*
void PCDX11RenderTerrain::ProcessSinglePassList(
	PCDX11RenderTerrain::RenderChunk *beginChunk,
	PCDX11RenderTerrain::RenderChunk *endChunk,
	PCDX11TerrainState *terrainState,
	uint32_t flags)
{
	// hack implementation
	uint32_t numChunks = endChunk - beginChunk;
	auto terrainChunks = new (renderDevice->linear34) TerrainChunk*[numChunks];
	for (uint32_t i=0; i<numChunks; i++) {
		terrainChunks[i] = numChunks[i]->m_pGeometry;
	}

	// HACK: instead of batching, this makes one drawable per chunk
	for (uint32_t i=0; i<numChunks; i++) {
		RenderTerrainGroup *group = CommonRenderTerrain::m_pGroups[chunk->m_pGeometry->m_groupIndex];
		auto *drawable = new (renderDevice, 0) PCDX11TerrainDrawable(
			group,
			terrainChunks+i,
			1,
			flags,
			terrainState);
		renderDevice->recordDrawable(drawable, group->renderPasses, false);
	}
}
*/

void PCDX11RenderTerrain::resFree() {
	// TODO
}

void PCDX11RenderTerrain::resFill(void* src, uint32_t size, uint32_t offset) {
	memcpy(((char*)m_pResourceData) + offset, src, size);
}

char *PCDX11RenderTerrain::resGetBuffer() {
	return (char*)m_pResourceData;
}

void PCDX11RenderTerrain::resConstruct() {

	printf("PCDX11RenderTerrain::resConstruct %p\n", this);

	RenderTerrainDataHeader *header = m_pHeader = m_pResourceData->pHeader;
	CommonMaterial **materials = m_pResourceData->pMaterials->data;
	printf("  num materials %d\n", m_pResourceData->pMaterials->length);
	TextureMap **textures = m_pResourceData->pTextures->data;
	printf("  num textures %d\n", m_pResourceData->pTextures->length);

	m_pNodes = (IRenderTerrain::Node*)(((char*)header) + header->nodeOffset);
	m_pGroups = (RenderTerrainGroup*)(((char*)header) + header->groupOffset);

	for (uint32_t i = 0; i < header->numGroups; i++) {
		RenderTerrainGroup *group = &m_pGroups[i];
		group->m_pInstanceTextures[0] = group->m_instanceTextureIndices[0] != -1
			? textures[group->m_instanceTextureIndices[0]] : nullptr;
		group->m_pInstanceTextures[1] = group->m_instanceTextureIndices[1] != -1
			? textures[group->m_instanceTextureIndices[1]] : nullptr;
		group->m_pInstanceTextures[2] = group->m_instanceTextureIndices[2] != -1
			? textures[group->m_instanceTextureIndices[2]] : nullptr;
		group->m_pInstanceTextures[3] = group->m_instanceTextureIndices[3] != -1
			? textures[group->m_instanceTextureIndices[3]] : nullptr;
		printf("  material %d -> %p\n", group->materialIndex, materials[group->materialIndex]);
		group->m_pMaterial = materials[group->materialIndex];
		group->renderPasses = // 0x52CB
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
			(1 << kPassIndexNonNormalDepth);

		// HACK: only these two are implemented right now
		group->renderPasses = // 0x1002
			(1 << kPassIndexComposite) |
			(1 << kPassIndexNormal);

		if (group->flags & 1)
			group->renderPasses = (1 << kPassIndexShadow); // 0x200
		if (group->flags & 2)
			group->renderPasses &= ~(1 << kPassIndexShadow); // ~0x200
	}

	m_pVertexBuffers = (RenderTerrainVertexBuffer*)(((char*)header) + header->vbOffset);

	// index layout information
	auto *layoutStream = (uint32_t*)(((char*)header) + header->layoutOffset);
	auto *layouts = new VertexAttributeLayoutA*[header->numLayouts];
	for (uint32_t i=0; i<header->numLayouts; i++) {
		layouts[i] = (VertexAttributeLayoutA*) layoutStream;
		layoutStream += 4 + 2 * layouts[i]->numAttr;
	}

	// create vertex buffers
	for (uint32_t i=0; i<header->numVertexBuffers; i++) {
		auto& buffer = m_pVertexBuffers[i];
		auto *layout = layouts[buffer.vertexDeclIndex];
		buffer.pVertexDecl = layout;

		void *vertices = (void*)(((char*)header) + buffer.primaryVertexDataOffset);
		auto *vertexBuffer = new PCDX11StaticVertexBuffer(
			PCDX11RenderExternalResource::renderDevice->staticPool10D10,
			layout->vertStrideA,
			buffer.numVertices,
			vertices);

		buffer.pPrimaryVertexBuffer = vertexBuffer;
	}

	// create index buffer
	auto *indices = (uint16_t*)(((char*)header) + header->indicesOffset);
	indexBuffer = new PCDX11StaticIndexBuffer(
		PCDX11RenderExternalResource::renderDevice->staticPool10D10,
		header->numIndices,
		indices);

	// convert offsets to pointers for TerrainChunkArray **m_ppGeometry
	union ChunkArrayOrOffset {
		uint32_t offset;
		TerrainChunkArray *ptr;
	};
	auto *chunkArrayOrOffset = (ChunkArrayOrOffset*)(((char*)header) + header->geomOffset);
	for (uint32_t i=0; i<header->numNodes; i++) {
		auto &caoo = chunkArrayOrOffset[i];
		if (caoo.offset)
			caoo.ptr = (TerrainChunkArray*)(((char*)header) + caoo.offset);
		else
			caoo.ptr = nullptr;
	}
	m_ppGeometry = (TerrainChunkArray**)chunkArrayOrOffset;

	// convert offset to pointer for unknown thing
	m_dword28 = (void*)(((char*)header) + header->dword3C);
}

}
