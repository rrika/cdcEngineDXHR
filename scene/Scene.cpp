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

SceneCellGroup *Scene::GetCellGroup(uint32_t index) {
	return sceneCellGroups[index];
}

SceneEntity *Scene::CreateEntity() {
	return new SceneEntity(this/*, TODO*/);	
}

bool Scene::SetupView(/*TODO*/) {
	// TODO
	return true;
}

void Scene::TraverseForView(/*TODO*/) {
	// TODO
	
	// HACK: just draw everything
	std::unordered_set<SceneEntity*> entities;
	for (auto group : sceneCellGroups) {
		for (uint32_t i = 0; i < group->getCellCount(); i++) {
			auto *cell = group->cellByIndex(i);
			for (uint16_t j = 0; j < cell->numSubCells; j++) {
				auto *subcell = &cell->subCells[j];
				for (auto ent : subcell->entities)
					entities.insert(ent);
			}
		}
	}

	for (auto ent : entities)
		activeEntities.push_back(ent);
}

void Scene::TraverseForLights(/*TODO*/) {
	// TODO
}

void Scene::CallCallbacks(/*TODO*/) {
	// TODO
}

void Scene::DrawTerrain(/*TODO*/) {
	// TODO
}

void Scene::DrawEntities(/*TODO*/) {
	// TODO
	for (auto entity: activeEntities) {
		// TODO
		entity->drawable->draw(&entity->matrix, /*TODO*/ 0.0f);
	}
}

void Scene::DoRender() {
	// TODO
	if (SetupView()) {
		TraverseForView();
		TraverseForLights();
		CallCallbacks();
		DrawTerrain();
		DrawEntities();
	}
	activeEntities.clear();
}

void Scene::Draw() {
	// TODO
	DoRender();
	// TODO
}

}
