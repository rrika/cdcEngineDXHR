#include "IDrawable.h"
#include "Scene.h"
#include "SceneCell.h"
#include "SceneCellGroup.h"
#include "SceneEntity.h"

namespace cdc {

SceneEntity::SceneEntity(Scene *scene) : scene(scene) {
	// called from SceneLayer::AddInstance
	scene->AddEntity(this);
}

void SceneEntity::ApplyUpdateState(UpdateState *updateState) {
	// called from UpdateInstances
	// without this, the object might still render in the correct position
	// but TransformVolumeAndPivot can't move the culling volume away from origin

	if (updateState->updateFlags & UpdateState::kEnabled) {
		enabled = updateState->enabled;
		// TODO
	}

	if (updateState->updateFlags & UpdateState::kDrawable) {
		// TODO
	}

	if (updateState->updateFlags & UpdateState::kCellGroup) {
		// TODO
	}

	if (updateState->updateFlags & UpdateState::kMatrix) {
		// TODO
		setMatrix(updateState->matrix);
	}

	if (updateState->updateFlags & UpdateState::kMoveState) {
		// TODO
	}

	if (updateState->updateFlags & (UpdateState::kUpdateVolume | UpdateState::kDrawable)) {
		// TODO
	}

	if (updateState->updateFlags & (UpdateState::kUpdateFlags | UpdateState::kDrawable)) {
		// TODO
	}
}

void SceneEntity::setMatrix(Matrix& newMatrix) {
	matrix = newMatrix;
	QueryVolumeFromDrawable();
	// TODO
}

Matrix& SceneEntity::getMatrix() {
	return matrix;
}

void SceneEntity::setDrawable(IDrawable *newDrawable) {
	drawable = newDrawable;
	QueryVolumeFromDrawable();
	// TODO
}

IDrawable *SceneEntity::getDrawable() {
	return drawable;
}

void SceneEntity::setCellGroup(ISceneCellGroup *newICellGroup) {
	auto newCellGroup = static_cast<SceneCellGroup*>(newICellGroup);
	if (sceneCellGroup != newCellGroup) {
		// TODO
		sceneCellGroup = newCellGroup;
		UpdateData(true);
	}
}

void SceneEntity::Release() {
	// called from SceneLayer::RemoveInstance
	delete this;
}

SceneEntity::~SceneEntity() {
	// TODO
	scene->RemoveEntity(this);
}

void SceneEntity::QueryVolumeFromDrawable() { // line 370
	if (drawable) {
		drawable->GetBoundingVolume(&cullingVolume);
		TransformVolumeAndPivot();
		// TODO
	} else {
		// TODO
	}
}

void SceneEntity::TransformVolumeAndPivot() { // line 406
	cullingVolume.Transform(matrix);
}

void SceneEntity::UpdateData(bool reInitCell) { // line 464
	// HACK
	auto cell = sceneCellGroup->cellByIndex(0);
	if (reInitCell) {
		auto subCell = &static_cast<SceneCell*>/*TODO*/(cell)->subCells[0];
		bool alreadyPresent = false;
		for (auto ent : subCell->entities)
			if (ent == this)
				alreadyPresent = true;
		if (!alreadyPresent)
			subCell->entities.push_back(this);
	}
}

}
