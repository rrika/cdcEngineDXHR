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
	// TODO
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

	PCDX11Material::setupMg21();
}

void PCDX11Scene::updateUniforms() {
	auto stateManager = deviceManager->getStateManager();
	auto& sceneBuffer = stateManager->accessCommonCB(1);
	// TODO
	stateManager->setViewMatrix(viewMatrix);
	stateManager->setProjectMatrix(projectMatrix);
	// TODO
	float row[4];
	row[0] = sub10.cameraPosition[0];
	row[1] = sub10.cameraPosition[1];
	row[2] = sub10.cameraPosition[2];
	row[3] = 0.0f;
	sceneBuffer.assignRow(10, row, 1); // SceneBuffer::CameraPosition
	row[0] = sub10.cameraDirection[0];
	row[1] = sub10.cameraDirection[1];
	row[2] = sub10.cameraDirection[2];
	row[3] = 0.0f;
	sceneBuffer.assignRow(11, row, 1); // SceneBuffer::CameraDirection
	row[0] = heightFogParams[0];
	row[1] = heightFogParams[1];
	row[2] = heightFogParams[2];
	row[3] = 0.0f;
	sceneBuffer.assignRow(25, row, 1); // SceneBuffer::HeightFogParams
	sceneBuffer.assignRow(9, fogScaleOffset, 1); // SceneBuffer::FogScaleOffset

	// HACK: needed for the composite submaterial to work
	row[0] = 0.0f;
	row[1] = 0.0f;
	row[2] = 0.0f;
	sceneBuffer.assignRow(27, row, 1); // SceneBuffer::GlobalParams[0] (ambient or rim light)
	row[0] = 0.0f;
	row[1] = 0.0f;
	row[2] = 0.0f;
	sceneBuffer.assignRow(28, row, 1); // SceneBuffer::GlobalParams[1] (how much pearl appearance)
	row[0] = 0.0f;
	sceneBuffer.assignRow(29, row, 1); // SceneBuffer::GlobalParams[2] (unsure what this does)
	row[0] = 0.0f;
	sceneBuffer.assignRow(33, row, 1); // SceneBuffer::GlobalParams[6] (unsure what this does)

	// HACK: needed for the normals submaterial to work
	row[0] = 1.0f;
	row[1] = 0.0f;
	row[2] = 0.0f;
	row[3] = 0.0f;
	sceneBuffer.assignRow(38, row, 1); // SceneBuffer::GlobalParams[11]
	row[0] = 0.0f;
	row[1] = 1.0f;
	sceneBuffer.assignRow(39, row, 1); // SceneBuffer::GlobalParams[12]
	row[1] = 0.0f;
	row[2] = 1.0f;
	sceneBuffer.assignRow(40, row, 1); // SceneBuffer::GlobalParams[13]
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
