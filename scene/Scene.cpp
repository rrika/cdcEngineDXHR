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
	sceneCellGroups.push_back(new SceneCellGroup(this, SceneCellGroup::kGlobal));
	sceneCellGroups.back()->allocateCells();
	sceneCellGroups.push_back(new SceneCellGroup(this, SceneCellGroup::kExterior));
	sceneCellGroups.back()->allocateCells();
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
