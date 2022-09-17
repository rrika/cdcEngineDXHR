#include "PCDX11RenderModelInstance.h"
#include "PCDX11MatrixState.h"
#include "PCDX11ModelDrawable.h"
#include "PCDX11RenderDevice.h"
#include <cstdio>

namespace cdc {

void PCDX11RenderModelInstance::recordDrawables(IMatrixState *matrixState) {
	// TODO
	uint32_t baseMask = this->baseMask;
	if (baseMask == 0)
		return;
	bool addToNextScene = false;
	Mesh *mesh = getRenderModel()->getMesh();
	// if (!mesh)
	// 	return; // HACK
	// if (!mesh->meshTable)
	// 	return; // HACK
	if (getRenderModel()->vsSelect == -1)
		return; // HACK
	uint32_t tab0index = 0;
	PoseData *poseData = static_cast<PCDX11MatrixState*>(matrixState)->poseData;

	cdc::CommonScene *scene = PCDX11RenderExternalResource::renderDevice->getScene();
	Matrix m = scene->getViewMatrix() * *(Matrix*)poseData->getMatrix(0);
	// HACK
	Vector d { m.m[3][0], m.m[3][1], m.m[3][2] };
	float dist = sqrtf(d.x*d.x + d.y*d.y + d.z*d.z);

	for (uint32_t i=0; i<mesh->meshCount; i++) {
		ModelBatch *sub = &mesh->meshTable[i];
		float fade = 1.0f;
		// printf("%f [%f %f %f %f]\n", dist, sub->minDist, sub->minFadeDist, sub->maxFadeDist, sub->maxDist);
		if (sub->minDist > dist || sub->maxDist < dist) {
			tab0index += sub->tab0EntryCount_30;
			continue;
		}
		if (sub->minFadeDist > dist) {
			fade = (dist - sub->minDist) / (sub->minFadeDist - sub->minDist);
		}
		else if (sub->maxFadeDist < dist) {
			fade = (sub->minDist - dist) / (sub->minDist - sub->minFadeDist);
		}

		for (uint32_t j=0; j<sub->tab0EntryCount_30; j++, tab0index++) {
			if (tab0index >= mesh->primGroupCount)
				return; // HACK: nothing good can come from this
			// printf("PCDX11RenderModelInstance::recordDrawables %d %d %d %p %p %p %p\n", i, j, tab0index, mesh, sub, mesh->meshTable, mesh->primGroups);
			PrimGroup *primGroup = &mesh->primGroups[tab0index];
			// NonPersistentPGData *tab0ext16 = &this->tab0Ext16[tab0index];
			PersistentPGData *tab0ext128 = &this->tab0Ext128[tab0index];

			uint32_t mask = baseMask; // & tab0ext16->mask8;
			auto drawable = new (renderDevice->getLinear(), 6, true) PCDX11ModelDrawable(
				getRenderModel(),
				ext,
				sub,
				primGroup,
				tab0ext128,
				poseData,
				fade);
			renderDevice->recordDrawable(drawable, mask, addToNextScene);
		}
	}
}

}