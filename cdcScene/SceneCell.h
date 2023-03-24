#pragma once
#include <cstdlib>
#include "ISceneCell.h"
#include "SceneCellContainer.h"
#include "SceneCellGroup.h"
#include "SceneSubCell.h"

namespace cdc {

class CommonRenderModelInstance;
class SceneSubCell;

class SceneCell :
	public ISceneCell,
	public SceneCellContainer
{
public:
	SceneSubCell *subCells = nullptr;
	uint16_t numSubCells = 0;
	// CellBlob *blob; // A8
	CommonRenderModelInstance *renderModelInstance = nullptr; // C8
public:
	SceneCell(SceneCellGroup *group) : SceneCellContainer(group->scene0, group) {
		// TODO
		subCells = new SceneSubCell[1];
		numSubCells = 1;
	}

	// 28 methods from ISceneCell
};

}
