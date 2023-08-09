#include "PCDeviceManager.h"
#include "PCScene.h"

namespace cdc {

void PCScene::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
	auto backupScene = renderDevice->scene78;
	renderDevice->scene78 = this;

	// TODO
	SetupConstants();
	// TODO
	if (drawableListsAndMasks) {
		drawableListsAndMasks->renderPasses->sortAndDraw(
			drawableListsAndMasks->passType,
			drawableListsAndMasks,
			renderDevice,
			drawableListsAndMasks->passMask8);
	}
	// TODO
	if (prevScene)
		prevScene->drawableListsAndMasks->absorbToBack(*drawableListsAndMasks);
	renderDevice->scene78 = backupScene;
}

void PCScene::SetupConstants() {
	// TODO
}

}
