#include "buffers/PCDX11UberConstantBuffer.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11Material.h"
#include "PCDX11Scene.h"
#include "PCDX11StateManager.h"
#include "PIXTracker.h"
#include "surfaces/PCDX11DepthBuffer.h"
#include "surfaces/PCDX11RenderTarget.h"

namespace cdc {

PCDX11Scene::PCDX11Scene(
	CommonRenderDevice *renderDevice,
	/* ... */
	CommonScene *parentScene,
	RenderViewport *viewport,
	CommonRenderTarget *renderTarget,
	CommonDepthBuffer *depthBuffer,
	/* ... */
	CommonRenderTarget *sourceColor,
	CommonDepthBuffer *sourceDepth,
	RenderGlobalState *globalState,
	/* ... */
	RenderPasses *renderPasses)
:
	CommonScene(renderDevice, parentScene, viewport, renderTarget, depthBuffer, sourceColor, sourceDepth, globalState, renderPasses),
	renderDevice(static_cast<PCDX11RenderDevice*>(renderDevice))
{
	if (!renderTarget && !depthBuffer) {
		this->renderTarget = this->renderDevice->GetDefaultRenderTarget(/*isFlush=*/ false);
		this->depthBuffer = this->renderDevice->GetDefaultDepthBuffer(/*isFlush=*/ false);
	}

	// TODO
	uint32_t mask = viewport->mask;

	if (mask & 1 << kPassIndexNormal)
		mask &= ~(1 << kPassIndexDepth);
	else
		mask &= ~(1 << kPassIndexNonNormalDepth);

	// TODO
	drawableListsAndMasks = renderPasses->createDrawableLists(kRegularPass, mask, renderDevice->getLinear());
}

void PCDX11Scene::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
	auto *otherScene = static_cast<PCDX11Scene*>(other);
	auto stateManager = deviceManager->getStateManager();
	auto backupScene = renderDevice->scene78;
	renderDevice->scene78 = this;

	PIXTracker::StartMarker(debugName.c_str());

	stateManager->pushRenderTargets(
		static_cast<PCDX11RenderTarget*>(renderTarget),
		static_cast<PCDX11DepthBuffer*>(depthBuffer));
	// TODO
	if (otherScene && otherScene->parentScene==nullptr)
		globalState.tex14[12] = otherScene->globalState.tex14[12];
	updateUniforms();
	auto& sceneBuffer = stateManager->accessCommonCB(1);
	sceneBuffer.assignRow(27, globalState.m_aParams, 16);
	// TODO

	if (sourceColor) {
		PIXTracker::StartMarker("color copy");
		stateManager->m_renderTarget->copyFromTexture(static_cast<PCDX11RenderTarget*>(sourceColor));
		PIXTracker::EndMarker();
	}
	if (sourceDepth)
		stateManager->m_depthBuffer->copyFromTexture(static_cast<PCDX11DepthBuffer*>(sourceDepth)); // not implemented in original binary

	if (drawableListsAndMasks) {
		drawableListsAndMasks->renderPasses->sortAndDraw(
			drawableListsAndMasks->passType,
			drawableListsAndMasks,
			renderDevice,
			drawableListsAndMasks->passMask8);
	}


	// HACK
	if (debugShowTempBuffer != -1) {
		if (TextureMap *temp = globalState.tex14[debugShowTempBuffer]) {
			PIXTracker::StartMarker("debugShowTempBuffer");
			renderDevice->copySurface(static_cast<PCDX11RenderTexture*>(temp), false, 15);
			PIXTracker::EndMarker();
		}
	}

	// TODO
	stateManager->popRenderTargets();
	if (prevScene)
		prevScene->drawableListsAndMasks->absorbToBack(*drawableListsAndMasks);
	renderDevice->scene78 = backupScene;

	PIXTracker::EndMarker();

	PCDX11Material::invalidate();
}

void PCDX11Scene::updateUniforms() {
	auto stateManager = deviceManager->getStateManager();
	auto& sceneBuffer = stateManager->accessCommonCB(1);
	// TODO
	stateManager->updateViewport();
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
	Matrix screenMatrix = {
		 2.f,  0.f, 0.f, 0.f,
		 0.f,  2.f, 0.f, 0.f,
		 0.f,  0.f, 1.f, 0.f,
		-1.f, -1.f, 0.f, 1.f,
	};
	sceneBuffer.assignMatrix(4, screenMatrix); // SceneBuffer::ScreenMatrix

	// for light shaders to translate the coordinates back
	float compressNear, compressFar;
	stateManager->getCompressedDepthRange(compressNear, compressFar); // assign 0.0f and 0.98f
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

	if (false) {
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
	}

	// HACK: this is done elsewhere
	memcpy(globalState.m_aParams + 44, viewMatrix.m[0], 16); // SceneBuffer::GlobalParams[11]
	memcpy(globalState.m_aParams + 48, viewMatrix.m[1], 16); // SceneBuffer::GlobalParams[12]
	memcpy(globalState.m_aParams + 52, viewMatrix.m[2], 16); // SceneBuffer::GlobalParams[13]
}

void PCDX11Scene::Finalize(DrawableList *drawableList) {
	// TODO
	FinalizeMiddle(drawableList, 0.f);
	// TODO
}

}
