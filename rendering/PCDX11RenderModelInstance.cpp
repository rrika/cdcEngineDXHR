#include "CommonMaterial.h"
#include "PCDX11Material.h"
#include "PCDX11MatrixState.h"
#include "PCDX11ModelDrawable.h"
#include "PCDX11RenderDevice.h"
#include "PCDX11RenderModelInstance.h"
#include <cstdio>
#include <cmath>

namespace cdc {

void PCDX11RenderModelInstance::setMaterial(uint32_t selector, IMaterial *material) {
	// TODO
	ModelData *mesh = renderMesh->getMesh();
	for (uint32_t i=0; i < mesh->primGroupCount; i++) {
		uint32_t primGroup20 = mesh->primGroups[i].dword20;
		if (selector == ~0u || selector == primGroup20 ||
			primGroup20 == 0 && selector == tab0Ext128[i].material->GetId())
			tab0Ext128[i].material = (PCDX11Material*)material;
	}
}

void PCDX11RenderModelInstance::recordDrawables(IMatrixState *matrixState) {
	// TODO
	uint32_t baseMask = this->baseMask;
	if (baseMask == 0)
		return;
	bool addToNextScene = false;
	ModelData *mesh = getRenderModel()->getMesh();
	// if (!mesh)
	// 	return; // HACK
	// if (!mesh->meshTable)
	// 	return; // HACK
	if (mesh->vsSelect == -1)
		return; // HACK
	uint32_t tab0index = 0;
	PoseData *poseData = static_cast<PCDX11MatrixState*>(matrixState)->poseData;

	cdc::CommonScene *scene = PCDX11RenderExternalResource::renderDevice->getScene();
	Matrix m = scene->getViewMatrix() * *(Matrix*)poseData->getMatrix(0);
	// HACK
	Vector d { m.m[3][0], m.m[3][1], m.m[3][2] };
	float dist = sqrtf(d.x*d.x + d.y*d.y + d.z*d.z);

	uint8_t flags = 0;
	{
		float *a = m.m[0];
		float *b = m.m[1];
		float *c = m.m[2];
		float cross[] = {
			a[1] * b[2] - a[2] * b[1],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - a[1] * b[0]
		};
		float sign = cross[0] * c[0] + cross[1] * c[1] + cross[2] * c[2];
		if (sign < 0.0f)
			flags |= 2;
	}

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
			NonPersistentPGData *tab0ext16 = &this->tab0Ext16[tab0index];
			PersistentPGData *tab0ext128 = &this->tab0Ext128[tab0index];

			if (tab0ext128->hide)
				continue;

			bool isFading = fade < 1.0f;
			uint32_t mask = baseMask & primGroup->material->GetRenderPassMask(isFading) & tab0ext16->mask8;
			float sortZ = dist; // HACK

			auto drawable = new (renderDevice->getLinear(), 6, true) PCDX11ModelDrawable(
				getRenderModel(),
				ext,
				sub,
				primGroup,
				tab0ext128,
				sortZ,
				poseData,
				fade,
				flags);
			renderDevice->recordDrawable(drawable, mask, addToNextScene);
		}
	}
}

}