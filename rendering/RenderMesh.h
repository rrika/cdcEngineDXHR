#pragma once
#include "RenderResource.h"

namespace cdc {

class CommonStreamDecl;
class PCDX11Material;
class PCDX11SimpleStaticVertexBuffer;

struct MeshTab0;

struct RenderFourTuple {
	uint32_t dword0;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
};

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
	uint32_t matrixGatherOffsets; // 38
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

struct Mesh { // guessed name
	uint32_t magic;
	MeshFlags flags;
	uint32_t dword8;
	uint32_t numIndices;
	uint32_t dword10;
	uint32_t dword14;
	uint32_t dword18;
	uint32_t dword1C;
	RenderFourTuple rft20;
	RenderFourTuple rft30;
	float dword40;
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

struct MeshTab0 {
	uint32_t vec0[4];
	uint32_t startIndex;
	uint32_t triangleCount;
	uint32_t dword18;
	uint32_t dword1C;
	uint32_t dword20;
	uint32_t dword24;
	void *material; // 28, index patched to pointer
	uint32_t dword2C; // index into fourBase, patched to pointer
	uint32_t dword30; // index into fourBase, patched to pointer
	uint32_t dword34; // index into fourBase, patched to pointer
	uint32_t dword38; // index into fourBase, patched to pointer
	uint32_t dword3C;
};

struct MeshTab0Ext16 {
	uint32_t dword0;
	float float4;
	uint32_t mask8;
	uint32_t dwordC;
};

struct MeshTab0Ext128Sub10 {
	// see also PCDX11Material::setupPixelResources
	uint32_t dword0;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	float float10;
	uint32_t dword14;
	uint32_t dword18;
	float minDepth;
	float maxDepth;
	CommonStreamDecl *streamDecls24[16];
	void *stencilSettings64;
};

struct MeshTab0Ext128 {
	PCDX11Material *material;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	MeshTab0Ext128Sub10 sub10;
	uint32_t dword78;
	uint32_t dword7C;
};

struct MeshHeader {
	Mesh *mesh;
	uint32_t *ptr4;
	void **materials;
	void *skeleton;
};

class RenderMesh : public RenderResource {
public:
	// void resRegisterAtRenderDevice() override;
	// void resFill(void* src, size_t size, size_t offset) override;
	// char *resGetBuffer() override;
	// void resConstruct() override;
	// void resMethod10();
	// dtor

	virtual void mesh18() = 0;
	virtual void mesh1C() = 0;
	virtual void mesh20(RenderFourTuple *a2, RenderFourTuple *a3) {};
	virtual void mesh24(float*, float*) = 0;
	virtual uint32_t getBoneCount() = 0;
	virtual void mesh2C() = 0;
	virtual MeshTab0Ext16 *getTab0Ext16() = 0;
	virtual MeshTab0Ext128 *getTab0Ext128() = 0;
	virtual Mesh *getMesh() = 0;
	virtual uint32_t getBoneCountIfEnabled() = 0;
	virtual void getSkeleton() = 0;
};

}
