#include "PCRenderModel.h"
#include <cstring>
#include <cstdio>
#include "rendering/CommonRenderModelInstance.h"
#include "PCMaterial.h"
#include "PCRenderDevice.h"
#include "buffers/PCStaticIndexBuffer.h"
#include "buffers/PCStaticVertexBuffer.h"
#include "rendering/VertexDeclaration.h"

namespace cdc {

PCRenderModel::PCRenderModel(uint32_t size, PCRenderDevice *renderDevice) :
	RenderMesh(),
	PCRenderExternalResource(renderDevice),
	size(size)
{
	meshHeader = (MeshHeader*)new char[size];
}

void PCRenderModel::resFill(void* src, uint32_t size, uint32_t offset) {
	memcpy((char*)meshHeader + offset, src, size);
}

char *PCRenderModel::resGetBuffer() {
	return (char*)meshHeader;
}

void PCRenderModel::resConstruct() {
	mesh = meshHeader->mesh;
	uint32_t *fourBase = meshHeader->ptr4 + 1;
	void **materials = meshHeader->materials + 1; // first dword is count
	char *magic = (char*)&mesh->magic;

	printf("reading mesh with magic \"%c%c%c%c\" (header %p) ", magic[0], magic[1], magic[2], magic[3], meshHeader);
	if (mesh->magic == 0x6873654D)
		printf("OK\n");
	else
		printf("NG\n");
	if (vsSelect == -1)
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
		auto *sub1 = &modelBatches[i];
		sub1->vertices += (uintptr_t)mesh;
		sub1->format += (uintptr_t)mesh;
		auto *layout = (VertexDecl*)sub1->format;
		auto *vertexBuffer = new PCStaticVertexBuffer(
			// PCRenderExternalResource::renderDevice->staticPool10D10
			nullptr);
		vertexBuffer->Create(
			(void*)sub1->vertices,
			sub1->numVertices,
			layout->vertStrideA);
		sub1->staticVertexBuffer = vertexBuffer;
		printf("%d:%d indices\n", i, sub1->numVertices);
		if (mesh->flags.hasBones)
			sub1->matrixGatherOffsets += (uintptr_t)mesh;
	}

	// process indices
	if (mesh->numIndices != 0) {
		printf("%d indices\n", mesh->numIndices);
		mesh->indices += (uintptr_t)mesh;
		indices = (uint16_t*) mesh->indices;
		indexBuffer = new PCStaticIndexBuffer(
			// PCRenderExternalResource::renderDevice->staticPool10D10,
			nullptr);
		indexBuffer->Create(
			(uint16_t*)indices,
			mesh->numIndices);
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
			tab0Ext128Byte[i].material = static_cast<PCMaterial*>(primGroups[i].material);
			tab0Ext128Byte[i].sub10.polyFlags = 0;
			tab0Ext128Byte[i].sub10.stencilSettings64 = nullptr;
			for (uint32_t j=0; j<16; j++)
				tab0Ext128Byte[i].sub10.streamDecls24[j] = nullptr;
		}
	}
}

}
