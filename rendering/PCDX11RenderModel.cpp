#include "PCDX11RenderModel.h"
#include <cstring>
#include <cstdio>
#include "CommonRenderModelInstance.h"
#include "PCDX11Material.h"
#include "buffers/PCDX11SimpleStaticIndexBuffer.h"
#include "buffers/PCDX11SimpleStaticVertexBuffer.h"
#include "VertexAttribute.h"

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

	printf("reading mesh with magic \"%c%c%c%c\" ", magic[0], magic[1], magic[2], magic[3]);
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
	mesh->table0 = (MeshTab0*)((uintptr_t)mesh + (uintptr_t)mesh->table0);
	table0 = mesh->table0;
	count0 = mesh->table0Count;

	for (uint32_t i = 0; i<count0; i++) {
		printf("  [%d] material %d -> ", i, (uintptr_t)table0[i].material);
		auto *material = materials[(uintptr_t)table0[i].material];
		table0[i].material = (IMaterial*)material;
		printf("%p\n", material);

		if (!material)
			allMaterialsPresent = false;

		if (table0[i].dword2C != -1)
			table0[i].dword2C = fourBase[table0[i].dword2C];

		if (table0[i].dword30 != -1)
			table0[i].dword30 = fourBase[table0[i].dword30];

		if (table0[i].dword34 != -1)
			table0[i].dword34 = fourBase[table0[i].dword34];

		if (table0[i].dword38 != -1)
			table0[i].dword38 = fourBase[table0[i].dword38];
	}

	// process table 1
	if (mesh->meshTable) {
		mesh->meshTable = (MeshSub*)((uintptr_t)mesh + (uintptr_t)mesh->meshTable);
		table1 = mesh->meshTable;
		count1 = mesh->meshCount;
	} else {
		count1 = 0;
	}

	for (uint32_t i = 0; i<count1; i++) {
		auto *sub1 = &table1[i];
		sub1->vertices += (uintptr_t)mesh;
		sub1->format += (uintptr_t)mesh;
		auto *layout = (VertexAttributeLayoutA*)sub1->format;
		sub1->staticVertexBuffer = new PCDX11SimpleStaticVertexBuffer( // should be PCDX11StaticVertexBuffer
			// RenderExternalResource::renderDevice->staticPool10D10,
			layout->vertStrideA,
			sub1->numVertices,
			(void*)sub1->vertices);
		printf("%d:%d indices\n", i, sub1->numVertices);
		if (mesh->flags.hasBones)
			sub1->matrixGatherOffsets += (uintptr_t)mesh;
	}

	// process indices
	if (mesh->numIndices != 0) {
		printf("%d indices\n", mesh->numIndices);
		mesh->indices += (uintptr_t)mesh;
		indices = mesh->indices;
		indexBuffer = new PCDX11SimpleStaticIndexBuffer( // should be PCDX11StaticIndexBuffer
			// RenderExternalResource::renderDevice->staticPool10D10,
			mesh->numIndices,
			(void*)indices);
	}

	// patch potential bones table
	if (mesh->flags.hasBones) {
		mesh->bonesTableMaybe += (uintptr_t)mesh;
	}

	// create additional tables
	if (count0) {
		tab0Ext128Byte = new PersistentPGData[count0];
		for (uint32_t i=0; i<count0; i++) {
			// TODO
		}
		tab0Ext16Byte = new NonPersistentPGData[count0];
		for (uint32_t i=0; i<count0; i++) {
			// TODO
			tab0Ext128Byte[i].material = static_cast<PCDX11Material*>(table0[i].material);
			tab0Ext128Byte[i].sub10.stencilSettings64 = nullptr;
			for (uint32_t j=0; j<16; j++)
				tab0Ext128Byte[i].sub10.streamDecls24[j] = nullptr;
		}
	}
}

}
