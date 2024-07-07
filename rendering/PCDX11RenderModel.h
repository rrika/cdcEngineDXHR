#pragma once
#include "RenderMesh.h"
#include "PCDX11RenderExternalResource.h"

namespace cdc {

class PCDX11RenderDevice;
class PCDX11StaticIndexBuffer;

class PCDX11RenderModel :
	public RenderMesh,
	public PCDX11RenderExternalResource
{
public:
	uint32_t size;

	MeshHeader *meshHeader;
	ModelData  *mesh = nullptr;

	uint16_t numPrimGroups = 0; // 1C
	PrimGroup *primGroups = nullptr; // 20

	uint16_t numModelBatches; // 24
	ModelBatch *modelBatches; // 28

	// uint32_t bonesMaybe;
	// uint32_t dword30;
	uint32_t indices;
	PCDX11StaticIndexBuffer *indexBuffer; // 38
	NonPersistentPGData *tab0Ext16Byte = nullptr; // one per primgroup
	PersistentPGData *tab0Ext128Byte = nullptr; // one per primgroup

public:
	PCDX11RenderModel(PCDX11RenderDevice *renderDevice, uint32_t size);

	void resFree() override { /*TODO*/ }
	void resFill(void* src, uint32_t size, uint32_t offset) override;
	char *resGetBuffer() override;
	void resConstruct() override;
	// void resMethod10();
	// dtor

	void mesh18() override { /*TODO*/ }
	void getBoundingSphere(float*, float*) const override { /*TODO*/ }
	bool getBoundingBox(Vector3&, Vector3&) const override;
	void getLodDistances(float*, float*) override { /*TODO*/ }
	uint32_t getBoneCount() const override { return mesh ? mesh->boneCountMaybe : 0; }
	void mesh2C() override { /*TODO*/ }
	NonPersistentPGData *getTab0Ext16() override { /*TODO*/ return tab0Ext16Byte; }
	PersistentPGData *getTab0Ext128() override { /*TODO*/ return tab0Ext128Byte; }
	ModelData *getMesh() override { /*TODO*/ return mesh; }
	uint32_t getBoneCountIfEnabled() override { /*TODO*/ return 0; }
	void getSkeleton() override { /*TODO*/ }
};

}
