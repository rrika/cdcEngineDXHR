#pragma once
#include "ISceneCell.h"
#include "SceneCellContainer.h"
#include "SceneCellGroup.h"

namespace cdc {

class CommonRenderModelInstance;

class SceneCell :
	public ISceneCell,
	public SceneCellContainer
{
	// CellBlob *blob; // A8
	CommonRenderModelInstance *renderModelInstance = nullptr; // C8
public:
	SceneCell(SceneCellGroup *group) : SceneCellContainer(group->scene0, group) {
		// TODO
	}

	// 28 methods from ISceneCell
};

}
