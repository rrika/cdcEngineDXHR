#include "SceneLayer.h"
#include "../scene/SceneManager.h"

namespace cdc {
	class CommonRenderDevice;
	extern CommonRenderDevice *g_renderDevice;
}

cdc::Scene *g_scene = nullptr;

class MyStreamingCallback : public StreamingCallback { // 236
public:
	// void UnitLoaded(StreamUnit*) override;
	// void UnitDumped(StreamUnit*) override;
	// void StreamGroupLoaded(cdc::CellStreamGroupData*) override;
	// void StreamGroupDumped(cdc::CellStreamGroupData*) override;
};

void SceneLayer::init() { // 1001
	auto *sceneManager = cdc::SceneManager::create();
	sceneManager->createScene(cdc::g_renderDevice);
}

class ImfStreamingCallback : public StreamingCallback { // 3089
public:
	// void UnitLoaded(StreamUnit*) override;
	// void UnitDumped(StreamUnit*) override;
	// void StreamGroupLoaded(cdc::CellStreamGroupData*) override;
	// void StreamGroupDumped(cdc::CellStreamGroupData*) override;
};
