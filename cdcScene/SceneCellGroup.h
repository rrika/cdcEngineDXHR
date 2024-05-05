#pragma once
#include <cstdint>
#include <vector>
#include "cdcScene.h"
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
	IScene *getScene();
	uint32_t getCellCount();
	ISceneCell *cellByIndex(uint32_t index);
	ISceneCell *queryPoint(float *point, bool useThisContainer);
	float *getOrigin();
	void setUserData(void *);
	void setName(const char *newName);
	const char *getName();
};

}
