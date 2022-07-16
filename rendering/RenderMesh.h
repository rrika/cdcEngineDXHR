#pragma once
#include "RenderResource.h"

namespace cdc {

class IMaterial;
class PCDX11SimpleStaticVertexBuffer;
// struct StencilSettings;
struct NonPersistentPGData;
struct PersistentPGData;

struct MeshTab0;

struct MeshSub {
	float minDistMaybe0;
	float maxDistMaybe4;
	float lowFadeDistMaybe8;
	float highFadeDistMaybeC;
	uint32_t dword10;
	uint32_t dword14;
	uint32_t dword18;
	uint32_t dword1C;
	uint32_t dword20;
	uint32_t dword24;
	uint32_t dword28;
	uint32_t dword2C;
	uint32_t tab0EntryCount_30;
	uint16_t commonCb3_numMatrices;
	uint16_t word36;
	uint32_t *matrixGatherOffsets; // 38
	uint32_t vertices; // 3C, offset patched to pointer
	// PCDX11StaticVertexBuffer *staticVertexBuffer;
	PCDX11SimpleStaticVertexBuffer *staticVertexBuffer; // hack
	uint32_t dword44;
	uint32_t dword48;
	uint32_t format; // 4C, offset patched to pointer
	uint32_t numVertices;
	uint32_t indexOffsetProbably;
	uint32_t numTrianglesProbably;
	uint32_t dword5C;
};

struct MeshFlags {
	uint32_t hasBones : 1;
	uint32_t depthLayer : 1;
};

struct Mesh { // = cdc::ModelData
	uint32_t magic;
	MeshFlags flags;
	uint32_t dword8; // totalDataSize
	uint32_t numIndices;
	float boundingSphereCenter[4];
	float boundingBoxMin[4];
	float boundingBoxMax[4];
	float boundingSphereRadius;
	float dword44;
	float dword48;
	uint32_t vsSelect4C;
	uint32_t matTableMaybe; // 50
	MeshTab0 *table0; // 54
	MeshSub *meshTable; // 58
	uint32_t bonesTableMaybe; // 5C
	uint32_t indices; // 60
	uint16_t table0Count; // 64
	uint16_t meshCount; // 66
	uint16_t boneCountMaybe; // 68
	uint16_t word6A;
	uint32_t dword6C;
	uint32_t dword70; // offset from beginning of struct, patched to pointer
	uint32_t dword74;
	uint32_t dword78;
	uint32_t dword7C;
};

struct MeshTab0 { // = cdc::PrimGroup
	uint32_t vec0[4];
	uint32_t startIndex;
	uint32_t triangleCount;
	uint32_t dword18;
	uint32_t dword1C;
	uint32_t dword20;
	uint32_t dword24;
	IMaterial *material; // 28, index patched to pointer
	uint32_t dword2C; // index into fourBase, patched to pointer
	uint32_t dword30; // index into fourBase, patched to pointer
	uint32_t dword34; // index into fourBase, patched to pointer
	uint32_t dword38; // index into fourBase, patched to pointer
	uint32_t dword3C;
};

struct MeshHeader {
	Mesh *mesh;
	uint32_t *ptr4;
	void **materials;
	void *skeleton;
};

class RenderMesh : public RenderResource {
public:
	// void resFree() override;
	// void resFill(void* src, size_t size, size_t offset) override;
	// char *resGetBuffer() override;
	// void resConstruct() override;
	// void resMethod10();
	// dtor

	virtual void mesh18() = 0;
	virtual void getBoundingSphere(float*, float*) = 0; // 1C
	virtual void getBoundingBox(float*, float*) {}; // 20
	virtual void getLodDistances(float*, float*) = 0; // 24
	virtual uint32_t getBoneCount() = 0; // 28
	virtual void mesh2C() = 0;
	virtual NonPersistentPGData *getTab0Ext16() = 0;
	virtual PersistentPGData *getTab0Ext128() = 0;
	virtual Mesh *getMesh() = 0;
	virtual uint32_t getBoneCountIfEnabled() = 0;
	virtual void getSkeleton() = 0;
};

}
