#include "PCDX11RenderModel.h"
#include <cstring>
#include <cstdio>
#include "CommonRenderModelInstance.h"
#include "PCDX11Material.h"
#include "PCDX11RenderDevice.h"
#include "buffers/PCDX11StaticIndexBuffer.h"
#include "buffers/PCDX11StaticVertexBuffer.h"
#include "VertexDeclaration.h"

namespace cdc {

PCDX11RenderModel::PCDX11RenderModel(PCDX11RenderDevice *renderDevice, uint32_t size) :
	RenderMesh(),
	PCDX11RenderExternalResource(renderDevice),
	size(size)
{
	meshHeader = (MeshHeader*)new char[size];
}

void PCDX11RenderModel::resFill(void* src, uint32_t size, uint32_t offset) {
	memcpy((char*)meshHeader + offset, src, size);
}

char *PCDX11RenderModel::resGetBuffer() {
	return (char*)meshHeader;
}

void PCDX11RenderModel::resConstruct() {
	mesh = meshHeader->mesh;
	uint32_t *fourBase = meshHeader->ptr4 + 1;
	void **materials = meshHeader->materials + 1; // first dword is count
	char *magic = (char*)&mesh->magic;

	printf("reading mesh with magic \"%c%c%c%c\" (header %p) ", magic[0], magic[1], magic[2], magic[3], meshHeader);
	if (mesh->magic == 0x6873654D)
		printf("OK\n");
	else
		printf("NG\n");
	if (mesh->vsSelect == -1)
		return;

	printf(" header.mesh      = %p\n", meshHeader->mesh);
	printf(" header.ptr4      = %p\n", meshHeader->ptr4);
	printf(" header.materials = %p\n", meshHeader->materials);
	printf(" header.skeleton  = %p\n", meshHeader->skeleton);

	bool allMaterialsPresent = true;
	if (mesh->dword70)
		mesh->dword70 += (uintptr_t)mesh;

	// process table 0
	mesh->primGroups = (PrimGroup*)((uintptr_t)mesh + (uintptr_t)mesh->primGroups);
	primGroups = mesh->primGroups;
	numPrimGroups = mesh->primGroupCount;

	printf(" mesh.primGroupCount = %d\n", numPrimGroups);

	for (uint32_t i = 0; i<numPrimGroups; i++) {
		printf("  [%d] material %d -> ", i, (uintptr_t)primGroups[i].material);
		auto *material = materials[(uintptr_t)primGroups[i].material];
		primGroups[i].material = (IMaterial*)material;
		printf("%p\n", material);

		if (!material)
			allMaterialsPresent = false;

		if (primGroups[i].dword2C != -1)
			primGroups[i].dword2C = fourBase[primGroups[i].dword2C];

		if (primGroups[i].dword30 != -1)
			primGroups[i].dword30 = fourBase[primGroups[i].dword30];

		if (primGroups[i].dword34 != -1)
			primGroups[i].dword34 = fourBase[primGroups[i].dword34];

		if (primGroups[i].dword38 != -1)
			primGroups[i].dword38 = fourBase[primGroups[i].dword38];
	}

	// process table 1
	if (mesh->meshTable) {
		mesh->meshTable = (ModelBatch*)((uintptr_t)mesh + (uintptr_t)mesh->meshTable);
		modelBatches = mesh->meshTable;
		numModelBatches = mesh->meshCount;
	} else {
		numModelBatches = 0;
	}

	for (uint32_t i = 0; i<numModelBatches; i++) {
		ModelBatch *sub1 = &modelBatches[i];
		sub1->vertices += (uintptr_t)mesh;
		sub1->format += (uintptr_t)mesh;
		auto *layout = (VertexDecl*)sub1->format;
		sub1->staticVertexBuffer = new PCDX11StaticVertexBuffer(
			PCDX11RenderExternalResource::renderDevice->m_pStaticVertexPool,
			layout->vertStrideA,
			sub1->numVertices,
			(void*)sub1->vertices);
		printf("%d:%d indices\n", i, sub1->numVertices);
		if (mesh->flags.hasBones)
			sub1->matrixGatherOffsets = (uint32_t*)((uintptr_t)mesh + (uintptr_t)sub1->matrixGatherOffsets);
	}

	// process indices
	if (mesh->numIndices != 0) {
		printf("%d indices\n", mesh->numIndices);
		mesh->indices += (uintptr_t)mesh;
		indices = mesh->indices;
		indexBuffer = new PCDX11StaticIndexBuffer(
			PCDX11RenderExternalResource::renderDevice->m_pStaticIndexPool,
			mesh->numIndices,
			(void*)indices);
	}

	// patch potential bones table
	if (mesh->flags.hasBones) {
		mesh->bonesTableMaybe += (uintptr_t)mesh;
	}

	// create additional tables
	if (numPrimGroups) {
		tab0Ext128Byte = new PersistentPGData[numPrimGroups];
		for (uint32_t i=0; i<numPrimGroups; i++) {
			// TODO
		}
		tab0Ext16Byte = new NonPersistentPGData[numPrimGroups];
		for (uint32_t i=0; i<numPrimGroups; i++) {
			// TODO
			tab0Ext128Byte[i].material = static_cast<PCDX11Material*>(primGroups[i].material);
			tab0Ext128Byte[i].sub10.polyFlags = 0;
			tab0Ext128Byte[i].sub10.stencilSettings64 = nullptr;
			for (uint32_t j=0; j<16; j++)
				tab0Ext128Byte[i].sub10.streamDecls24[j] = nullptr;

			tab0Ext128Byte[i].hide = false; // HACK

			tab0Ext16Byte[i].mask8 = 0xffffffff;
			uint8_t f = primGroups[i].flags | (mesh->flags.depthLayer ? 2 : 0);
			if (f & 2)
				tab0Ext16Byte[i].mask8 &= ~(1 << kPassIndex8);
			if (f & 1) // probably shadow only
				tab0Ext16Byte[i].mask8 &= ~(// 0x50CB
					(1 << kPassIndexDepth) |
					(1 << kPassIndexComposite) |
					// (1 << kPassIndexOpaque) |
					(1 << kPassIndexTranslucent) |
					// (1 << kPassIndexFullScreenFX) |
					// (1 << kPassIndexPostFSX) |
					(1 << kPassIndexAlphaBloomFSX) |
					(1 << kPassIndexPredator) |
					// (1 << kPassIndex8) |
					// (1 << kPassIndexShadow) |
					// (1 << kPassIndexDepthDependent) |
					(1 << kPassIndexNormal) |
					// (1 << kPassIndexDeferredShading) | /* things look nicer when pure shadow casters don't run on this pass */
					(1 << kPassIndexNonNormalDepth));
		}
	}
}

bool PCDX11RenderModel::getBoundingBox(Vector3& min, Vector3& max) const {
	if (!mesh)
		return false;

	min.x = mesh->boundingBoxMin[0];
	min.y = mesh->boundingBoxMin[1];
	min.z = mesh->boundingBoxMin[2];
	min.w = mesh->boundingBoxMin[3];

	max.x = mesh->boundingBoxMax[0];
	max.y = mesh->boundingBoxMax[1];
	max.z = mesh->boundingBoxMax[2];
	max.w = mesh->boundingBoxMax[3];
	return true;
}

}
