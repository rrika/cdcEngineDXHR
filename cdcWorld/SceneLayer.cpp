#include <cstdint>
#include "SceneLayer.h"
#include "cdcScene/SceneManager.h"

namespace cdc {
	class CommonRenderDevice;
	extern CommonRenderDevice *g_renderDevice;
}

cdc::IScene *g_scene = nullptr;

class MyStreamingCallback : public StreamingCallback { // 236
public:
	// void UnitLoaded(StreamUnit*) override;
	// void UnitDumped(StreamUnit*) override;
	// void StreamGroupLoaded(cdc::CellStreamGroupData*) override;
	// void StreamGroupDumped(cdc::CellStreamGroupData*) override;
};

void SceneLayer::init() { // 1001
	auto *sceneManager = cdc::SceneManager::create();
	g_scene = sceneManager->createScene(cdc::g_renderDevice);
}

uint32_t s_numStreamingCallbacks = 0;
StreamingCallback *s_streamingCallbacks[4];

void SceneLayer::AddStreamingCallback(StreamingCallback *cb) { // 1083
	if (cb && s_numStreamingCallbacks < 4) {
		s_streamingCallbacks[s_numStreamingCallbacks++] = cb;
	}
}

void SceneLayer::RemoveStreamingCallback(StreamingCallback *cb) { // 1093
	for (uint32_t i = 0; i < s_numStreamingCallbacks; i++) {
		if (s_streamingCallbacks[i] == cb) {
			// swap-delete with last cb
			s_streamingCallbacks[i] = s_streamingCallbacks[--s_numStreamingCallbacks];
			return;
		}
	}
}

void SceneLayer::PreStreamIn(StreamUnit*) { // 1282
	// TODO
}

void SceneLayer::PostStreamIn(StreamUnit *unit) { // 1443
	// TODO
	for (uint32_t i=0; i < s_numStreamingCallbacks; i++)
		s_streamingCallbacks[i]->UnitLoaded(unit);
}

class ImfStreamingCallback : public StreamingCallback { // 3089
public:
	// void UnitLoaded(StreamUnit*) override;
	// void UnitDumped(StreamUnit*) override;
	// void StreamGroupLoaded(cdc::CellStreamGroupData*) override;
	// void StreamGroupDumped(cdc::CellStreamGroupData*) override;
};
