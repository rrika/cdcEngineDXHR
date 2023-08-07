#pragma once
#include <cstdint>
#include "rendering/IRenderDrawable.h"
#include "rendering/CommonRenderModelInstance.h" // for PersistentPGData and RenderModelInstanceData
#include "rendering/RenderMesh.h" // for ModelBatch and PrimGroup

namespace cdc {

class PCRenderModel;
class PCRenderDevice;
class PCStateManager;
class PCStreamDecl;
class PCPoseData;
struct LightReceiverData;

class PCModelDrawable /*: public IRenderDrawable*/ {

	PCRenderModel *renderModel; // C
	RenderModelInstanceData *ext; // 10
	ModelBatch *modelBatch; // 14
	PrimGroup *primGroup; // 18
	PersistentPGData *persistentPG; // 1C
	PCPoseData *poseData; // 20
	uint32_t dword24;
	uint32_t dword28;
	float lodOpacity; // 2C
	uint32_t flags; // 30

public:
	PCModelDrawable(
		PCRenderModel *renderModel,
		RenderModelInstanceData *ext,
		ModelBatch *modelBatch,
		PrimGroup *primGroup,
		PersistentPGData *persistentPG,
		PCPoseData *poseData,
		uint32_t numPrimGroups,
		float lodOpacity,
		uint32_t flags);

	// void draw(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) override;
	// uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) override;

	void DrawPrimitive(
		PCRenderDevice *renderDevice,
		PCStateManager *stateManager,
		PCStreamDecl *streamDecl,
		bool renderTwice);
	/*bool setMatrices(
		PCStateManager *stateManager,
		PCModelDrawable *prevDrawable,
		bool hasBones);*/
};

}
