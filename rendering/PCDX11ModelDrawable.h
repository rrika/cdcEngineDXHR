#pragma once
#include "IRenderDrawable.h"
#include "CommonRenderModelInstance.h" // for MeshTab0Ext128
#include "RenderMesh.h" // for MeshSub and MeshTab0

namespace cdc {

class PCDX11RenderModel;
class PCDX11RenderDevice;
class PCDX11StateManager;
class PCDX11StreamDecl;
class PoseData;
struct LightReceiverData;

struct ModelDrawableExt {
	// TODO
	uint32_t dword50;
};

class PCDX11ModelDrawable : public IRenderDrawable {

	PCDX11RenderModel *renderModel; // C
	ModelDrawableExt *ext; // 10
	MeshSub *meshSub; // 14
	MeshTab0 *tab0; // 18
	MeshTab0Ext128 *tab0Ext128; // 1C
	PoseData *poseData; // 20
	LightReceiverData *lightReceiverData; // 24
	float *lightConstantBufferData; // 28
	float float2C; // 2C
	float float30; // 30
	uint32_t flags34; // 34

public:
	PCDX11ModelDrawable(
		PCDX11RenderModel *renderModel,
		MeshSub *meshSub,
		MeshTab0 *tab0,
		MeshTab0Ext128 *tab0Ext128,
		PoseData *poseData);

	inline bool isUnlit() const { return (flags34 >> 0) & 1; }
	inline bool getCullMode() const { return (flags34 >> 1) & 1; }
	inline uint32_t getTriangleCount() const { return (flags34 >> 8); }

	static void draw1(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void draw2(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void draw7(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void draw4(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void draw56(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static void drawA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);

	static bool compare128(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool compare7(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool compare46(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool compareA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);

	void draw(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) override;
	uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) override;

	void draw(
		PCDX11RenderDevice *renderDevice,
		PCDX11StateManager *stateManager,
		PCDX11StreamDecl *streamDecl,
		bool renderTwice);
	bool setMatrices(
		PCDX11StateManager *stateManager,
		PCDX11ModelDrawable *prevDrawable,
		bool hasBones);
	void buildAndAssignLightBuffer(
		PCDX11RenderDevice *renderDevice,
		PCDX11StateManager *stateManager);
};

}