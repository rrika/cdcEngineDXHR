#pragma once
#include <cstdint>
#include <vector>
#include "ISceneCellGroup.h"
#include "SceneCellContainer.h"

namespace cdc {

class CellGroupData;
class Scene;
class SceneCell;

class SceneCellGroup :
	public ISceneCellGroup,
	public SceneCellContainer
{
	Scene *scene1C; // 1C
	std::vector<SceneCell*> cells; // 20
	CellGroupData *cellGroupData; // 90
	// CellGroupUserData *userData; // 94
	float origin[4]; // B0
	const char *name; // C0
public:

	enum SpecialType {
		kGlobal,
		kExterior
	};

	SceneCellGroup(Scene*, SpecialType);
	SceneCellGroup(Scene*, CellGroupData*);

	void allocateCells();

	// 22 methods from ISceneCellGroup
	Scene *getScene() override { return scene1C; } // 4
	uint32_t getCellCount() override { return cells.size(); } // 8
	SceneCell *cellByIndex(uint32_t index) override { return cells[index]; } // 10
	SceneCell *queryPoint(float *point, bool useThisContainer) override { // 14
		return SceneCellContainer::queryPoint(point, useThisContainer);
	}
	float *getOrigin() override { return origin; } // 20
	void setUserData(void *) override { // 38
		// TODO
	}
	void setName(const char *newName) override { name = newName; } // 48
	const char *getName() override { return name; } // 4C
};

}
