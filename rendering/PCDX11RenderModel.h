#pragma once
#include "RenderMesh.h"
#include "PCDX11RenderExternalResource.h"

namespace cdc {

class PCDX11RenderDevice;
class PCDX11SimpleStaticIndexBuffer;

class PCDX11RenderModel :
	public RenderMesh,
	public PCDX11RenderExternalResource
{
public:
	uint32_t size;

	MeshHeader *meshHeader;
	Mesh       *mesh;

	uint16_t count0; // 1C
	PrimGroup *primGroups; // 20

	uint16_t count1; // 24
	MeshSub *table1; // 28

	// uint32_t bonesMaybe;
	// uint32_t dword30;
	uint32_t indices;
	// PCDX11StaticIndexBuffer *indexBuffer; // 38
	PCDX11SimpleStaticIndexBuffer *indexBuffer; // 38, hack
	NonPersistentPGData *tab0Ext16Byte; // uses count0
	PersistentPGData *tab0Ext128Byte; // uses count0
	uint32_t vsSelect; // 4C

public:
	PCDX11RenderModel(PCDX11RenderDevice *renderDevice, uint32_t size);

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
