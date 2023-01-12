#include "SceneCell.h"
#include "SceneCellGroup.h"
#include "SceneEntity.h"

namespace cdc {

void SceneEntity::setMatrix(Matrix& newMatrix) {
	matrix = newMatrix;
	// TODO
}

Matrix& SceneEntity::getMatrix() {
	return matrix;
}

void SceneEntity::setDrawable(IDrawable *newDrawable) {
	drawable = newDrawable;
	// TODO
}

IDrawable *SceneEntity::getDrawable() {
	return drawable;
}

}
