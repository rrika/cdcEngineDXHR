#pragma once
#include "ISceneEntity.h"
#include "cdcMath/Math.h"

namespace cdc {

class IDrawable;
class ISceneCellGroup;
class Scene;
class SceneCellGroup;

class SceneEntity : public ISceneEntity {
public:
	Scene *scene; // 4
	Matrix matrix; // 20
	IDrawable *drawable = nullptr; // 6C
	SceneCellGroup *sceneCellGroup = nullptr; // 70

	void UpdateData(bool);

public:
	SceneEntity(Scene *scene) : scene(scene) {}

	// 25 methods
	void setMatrix(Matrix&) override;
	Matrix& getMatrix() override;
	void setDrawable(IDrawable *) override;
	IDrawable *getDrawable() override;
	void setCellGroup(ISceneCellGroup *) override;
};

}
