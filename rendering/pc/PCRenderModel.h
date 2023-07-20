#pragma once
#include "rendering/RenderMesh.h"
#include "PCRenderExternalResource.h"

namespace cdc {

class PCRenderDevice;
class PCStaticIndexBuffer;

class PCRenderModel :
	public RenderMesh,
	public PCRenderExternalResource
{
public:
	uint32_t size;

	MeshHeader *meshHeader; // 14
	Mesh       *mesh = nullptr; // 18

	uint16_t numPrimGroups = 0; // 1C
	PrimGroup *primGroups = nullptr; // 20

	uint16_t numModelBatches; // 24
	ModelBatch *modelBatches; // 28

	// uint32_t dword2C;
	// uint32_t dword30;
	uint16_t *indices;
	PCStaticIndexBuffer *indexBuffer; // 38
	NonPersistentPGData *tab0Ext16Byte = nullptr; // one per primgroup
	PersistentPGData *tab0Ext128Byte = nullptr; // one per primgroup
	uint32_t vsSelect; // 4C

public:
	PCRenderModel(uint32_t size, PCRenderDevice *renderDevice);

	void resFree() override { /*TODO*/ }
	void resFill(void* src, uint32_t size, uint32_t offset) override;
	char *resGetBuffer() override;
	void resConstruct() override;
	// void resMethod10();
	// dtor

	void mesh18() override { /*TODO*/ }
	void getBoundingSphere(float*, float*) override { /*TODO*/ }
	// void getBoundingBox(float *, float *) { /*TODO*/ }
	void getLodDistances(float*, float*) override { /*TODO*/ }
	uint32_t getBoneCount() override { /*TODO*/ return 0; }
	void mesh2C() override { /*TODO*/ }
	NonPersistentPGData *getTab0Ext16() override { /*TODO*/ return tab0Ext16Byte; }
	PersistentPGData *getTab0Ext128() override { /*TODO*/ return tab0Ext128Byte; }
	Mesh *getMesh() override { /*TODO*/ return mesh; }
	uint32_t getBoneCountIfEnabled() override { /*TODO*/ return 0; }
	void getSkeleton() override { /*TODO*/ }
};

}
