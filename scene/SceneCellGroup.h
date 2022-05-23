#pragma once
#include <cstdint>
#include "ISceneCellGroup.h"
#include "SceneCellContainer.h"

namespace cdc {

class Scene;
class SceneCell;

class SceneCellGroup :
	public ISceneCellGroup,
	public SceneCellContainer
{
	Scene *scene1C; // 1C
	uint32_t numCells; // 20
	SceneCell **cells28; // 28
	// UnitBlobSub50Cells *sub5cells; // 90
	// CellGroupUserData *userData; // 94
public:
	// 22 methods from ISceneCellGroup
	SceneCell *queryPoint(float *point, bool useThisContainer) { // 14
		return SceneCellContainer::queryPoint(point, useThisContainer);
	}
};

}
