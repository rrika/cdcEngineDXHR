#include <unordered_set>
#include "IDrawable.h"
#include "Scene.h"
#include "SceneCell.h"
#include "SceneCellGroup.h"
#include "SceneEntity.h"

namespace cdc {

Scene::Scene(CommonRenderDevice *renderDevice) :
	renderDevice(renderDevice), sceneCell(nullptr)
{
	// TODO
}

SceneEntity *Scene::CreateEntity() {
	auto *newEntity = new SceneEntity(this/*, TODO*/);	
	activeEntities.push_back(newEntity); // HACK
	return newEntity;
}

void Scene::DrawEntities(/*TODO*/) {
	// TODO
	for (auto entity: activeEntities) {
		// TODO
		entity->drawable->draw(&entity->matrix, /*TODO*/ 0.0f);
	}
}

void Scene::Draw() {
	// TODO
	DrawEntities();
}

}
