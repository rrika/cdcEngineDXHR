#pragma once
#include "ISceneEntity.h"
#include "cdcMath/Math.h"
#include "rendering/Culling/BasicPrimitives.h"

namespace cdc {

class IDrawable;
class ISceneCellGroup;
class Scene;
class SceneCellGroup;

class SceneEntity : public ISceneEntity {
public:
	Scene *scene; // 4
	uint32_t entityIndex; // 8
	Matrix matrix; // 20
	IDrawable *drawable = nullptr; // 6C
	SceneCellGroup *sceneCellGroup = nullptr; // 70
	BasicCullingVolume cullingVolume; // F0

	void QueryVolumeFromDrawable();
	void TransformVolumeAndPivot();
	void UpdateData(bool);

public:
	SceneEntity(Scene *scene);

	// 25 methods
	void setMatrix(Matrix&) override;
	Matrix& getMatrix() override;
	void setDrawable(IDrawable *) override;
	IDrawable *getDrawable() override;
	void setCellGroup(ISceneCellGroup *) override;
};

}
