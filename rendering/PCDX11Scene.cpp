#include "buffers/PCDX11UberConstantBuffer.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11Material.h"
#include "PCDX11Scene.h"
#include "PCDX11StateManager.h"
#include "surfaces/PCDX11DepthBuffer.h"
#include "surfaces/PCDX11RenderTarget.h"

namespace cdc {

void PCDX11Scene::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
	auto stateManager = deviceManager->getStateManager();
	auto backupScene = renderDevice->scene78;
	renderDevice->scene78 = this;

	stateManager->pushRenderTargets(
		static_cast<PCDX11RenderTarget*>(renderTarget),
		static_cast<PCDX11DepthBuffer*>(depthBuffer));
	// TODO
	updateUniforms();
	auto& sceneBuffer = stateManager->accessCommonCB(1);
	sceneBuffer.assignRow(27, globalState.m_aParams, 16);
	// TODO

	if (sourceColor)
		stateManager->m_renderTarget->copyFromTexture(static_cast<PCDX11RenderTarget*>(sourceColor));
	if (sourceDepth)
		stateManager->m_depthBuffer->copyFromTexture(static_cast<PCDX11DepthBuffer*>(sourceDepth)); // not implemented in original binary

	if (drawableListsAndMasks) {
		drawableListsAndMasks->renderPasses->sortAndDraw(
			drawableListsAndMasks->passType,
			drawableListsAndMasks,
			renderDevice,
			drawableListsAndMasks->passMask8);
	}
	// TODO
	stateManager->popRenderTargets();
	if (prevScene)
		prevScene->drawableListsAndMasks->absorbToBack(*drawableListsAndMasks);
	renderDevice->scene78 = backupScene;

	PCDX11Material::invalidate();
}

void PCDX11Scene::updateUniforms() {
	auto stateManager = deviceManager->getStateManager();
	auto& sceneBuffer = stateManager->accessCommonCB(1);
	// TODO
	stateManager->setViewMatrix(viewMatrix);
	stateManager->setProjectMatrix(projectMatrix);
	// TODO
	float row[4];
	row[0] = viewport.viewMatrix.m[3][0];
	row[1] = viewport.viewMatrix.m[3][1];
	row[2] = viewport.viewMatrix.m[3][2];
	row[3] = 0.0f;
	sceneBuffer.assignRow(10, row, 1); // SceneBuffer::CameraPosition
	row[0] = viewport.viewMatrix.m[2][0];
	row[1] = viewport.viewMatrix.m[2][1];
	row[2] = viewport.viewMatrix.m[2][2];
	row[3] = 0.0f;
	sceneBuffer.assignRow(11, row, 1); // SceneBuffer::CameraDirection
	row[0] = heightFogParams[0];
	row[1] = heightFogParams[1];
	row[2] = heightFogParams[2];
	row[3] = 0.0f;
	sceneBuffer.assignRow(25, row, 1); // SceneBuffer::HeightFogParams
	sceneBuffer.assignRow(9, fogScaleOffset, 1); // SceneBuffer::FogScaleOffset

	// for light shaders to translate the coordinates back
	float compressNear = 0.0f, compressFar = 1.0f; // 0.98f;
	float compressRange = compressFar - compressNear;
	float p10 = projectMatrix.m[2][2]; // = f / (f - n)
	float p14 = projectMatrix.m[3][2]; // = n * f / (n - f)
	row[0] = 1.0f / (p14 * compressRange);
	row[1] = -(compressNear + p10 * compressRange) * row[0];
	row[2] = 1.0f / compressRange; // not used by light shaders I looked at
	row[3] = -compressNear / compressRange; // not used by light shaders I looked at
	//printf("DepthToW  %f %f %f %f\n", row[0], row[1], row[2], row[3]);
	sceneBuffer.assignRow(20, row, 1); // SceneBuffer::DepthToW

	float p0 = projectMatrix.m[0][0];
	float p5 = projectMatrix.m[1][1];
	float p8 = projectMatrix.m[2][0];
	float p9 = projectMatrix.m[2][1];
	row[0] = 2.0f / p0; // = width / near ?
	row[1] = -2.0f / p5; // = -height / near ?
	row[2] = -p8 - 1.0f / p0;
	row[3] = -p9 + 1.0f / p5;
	//printf("DepthToView  %f %f %f %f\n", row[0], row[1], row[2], row[3]);
	sceneBuffer.assignRow(18, row, 1); // SceneBuffer::DepthToView

	// HACK: needed for the composite submaterial to work
	row[0] = 0.1f;
	row[1] = 0.1f;
	row[2] = 0.1f;
	memcpy(globalState.m_aParams + 0, row, 16); // SceneBuffer::GlobalParams[0] (ambient or rim light)
	row[0] = 0.0f;
	row[1] = 0.0f;
	row[2] = 0.0f;
	memcpy(globalState.m_aParams + 4, row, 16); // SceneBuffer::GlobalParams[1] (how much pearl appearance)
	row[0] = 1.0f;
	memcpy(globalState.m_aParams + 8, row, 16); // SceneBuffer::GlobalParams[2] (unsure what this does)
	row[0] = 0.0f;
	memcpy(globalState.m_aParams + 24, row, 16); // SceneBuffer::GlobalParams[6] (unsure what this does)

	// HACK: needed for the normals submaterial to work
	row[0] = 1.0f;
	row[1] = 0.0f;
	row[2] = 0.0f;
	row[3] = 0.0f;
	memcpy(globalState.m_aParams + 44, row, 16); // SceneBuffer::GlobalParams[11]
	row[0] = 0.0f;
	row[1] = 1.0f;
	memcpy(globalState.m_aParams + 48, row, 16); // SceneBuffer::GlobalParams[12]
	row[1] = 0.0f;
	row[2] = 1.0f;
	memcpy(globalState.m_aParams + 52, row, 16); // SceneBuffer::GlobalParams[13]

	auto rt = stateManager->m_renderTarget;
	auto width = rt->getWidth();
	auto height = rt->getHeight();

	row[0] = 0.0f; // offset
	row[1] = 0.0f; // offset
	row[2] = 1.0f/width; // scale
	row[3] = 1.0f/height; // scale
	sceneBuffer.assignRow(44, row, 1); // SceneBuffer::ScreenExtents
	row[0] = width;
	row[1] = height;
	row[2] = 0.0f;
	row[3] = 0.0f;
	sceneBuffer.assignRow(45, row, 1); // SceneBuffer::ScreenResolution
}

void PCDX11Scene::addToDrawableList(DrawableList *drawableList) {
	// TODO
	addToDrawableList(drawableList, 0);
	// TODO
}

void PCDX11Scene::addToDrawableList(DrawableList *drawableList, uint32_t) {
	// TODO
	if (parentScene) {
		auto *parentDrawableList = parentScene->drawableListsAndMasks->listForPass(10);
		for (uint32_t i=0; i<numSubScenes; i++)
			parentDrawableList->add(subScenes[i]);
		// TODO
	} else {
		drawableList->add(this);
		for (uint32_t i=0; i<numSubScenes; i++)
			drawableList->add(subScenes[i]);
	}
}

}
