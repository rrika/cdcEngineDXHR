#include <cstdint>
#include "SceneLayer.h"
#include "cdcScene/IScene.h"
#include "cdcScene/SceneEntity.h"
#include "cdcScene/SceneManager.h"
#include "Instance.h"
#include "InstanceDrawable.h"

namespace cdc {
	class CommonRenderDevice;
	extern CommonRenderDevice *g_renderDevice;
}

using namespace cdc;

IScene *g_scene = nullptr;

class MyStreamingCallback : public StreamingCallback { // 236
public:
	// void UnitLoaded(StreamUnit*) override;
	// void UnitDumped(StreamUnit*) override;
	// void StreamGroupLoaded(CellStreamGroupData*) override;
	// void StreamGroupDumped(CellStreamGroupData*) override;
};

void SceneLayer::init() { // 1001
	auto *sceneManager = SceneManager::create();
	g_scene = sceneManager->createScene(g_renderDevice);
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

void SceneLayer::AddInstance(Instance *instance) { // 1998
	SceneEntity *entity = g_scene->CreateEntity();
	// entity->SetEnabled(false);
	// entity->SetName(instance->object->name)
	// entity->SetFlags(2, 0);
	instance->sceneEntity = entity;
	entity->setDrawable(instance->instanceDrawable);

	// TODO
}

static void UpdateInstances() { // 2052
	while (auto *id = InstanceDrawable::s_pFirstDirty) {
		Instance *i = id->m_instance;
		// TODO
		if (!i->sceneEntity)
			SceneLayer::AddInstance(i);

		// TODO

		if (i->sceneEntity) {
			SceneEntity::UpdateState us;
			us.updateFlags = SceneEntity::UpdateState::kMatrix /*8*/;

			// m_matrix[-1] is populated with the entity transform
			// m_matrix[0] is derived from it
			//   either as a copy of [-1] in G2Instance_SetTransformsToIdentity
			//   or by application of animation segments in G2Instance_RebuildTransforms

			auto& transformComponent = i->GetTransformComponent();
			us.matrix = transformComponent.m_matrix[0];

			if (id->QueryNoDraw()) {
				us.updateFlags |= SceneEntity::UpdateState::kEnabled /*1*/;
				us.enabled = false;
			} else {
				us.updateFlags |= SceneEntity::UpdateState::kEnabled /*1*/;
				us.enabled = true;
			}

			i->sceneEntity->ApplyUpdateState(&us);
		}

		id->RemoveFromDirtyList(); // via ValidateAll
	}
}

void SceneLayer::Update() {
	// TODO
	UpdateInstances();
	// TODO
}

class ImfStreamingCallback : public StreamingCallback { // 3089
public:
	// void UnitLoaded(StreamUnit*) override;
	// void UnitDumped(StreamUnit*) override;
	// void StreamGroupLoaded(cdc::CellStreamGroupData*) override;
	// void StreamGroupDumped(cdc::CellStreamGroupData*) override;
};
