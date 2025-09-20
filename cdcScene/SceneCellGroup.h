#pragma once
#include <cstdint>
#include <vector>
#include "cdcScene.h"
#include "SceneCell.h" // for covariant return type of GetCellFromPoint
#include "SceneCellContainer.h"

namespace cdc {

class CellGroupData;
class Scene;
class SceneCell;

class SceneCellGroup final :
	public ISceneCellGroup,
	public SceneCellContainer
{
public:
	Scene *m_pSceneB; // 1C, as opposed to the inherited m_pSceneA
	std::vector<SceneCell*> m_cells; // 20
	CellGroupData *m_pCellGroupData; // 90
	// CellGroupUserData *userData; // 94
	Vector3 origin; // B0
	const char *name; // C0
public:

	enum SpecialType {
		kGlobal,
		kExterior
	};

	SceneCellGroup(Scene*, SpecialType);
	SceneCellGroup(Scene*, CellGroupData*);

	void Init();

	// 22 methods from ISceneCellGroup
	IScene *getScene();
	uint32_t getCellCount();
	ISceneCell *cellByIndex(uint32_t index);
	SceneCell *GetCellFromPoint(Vector3 const& point, bool ignoreChildCellGroups);
	Vector3 *getOrigin();
	void setUserData(void *);
	void setName(const char *newName);
	const char *getName();
};

}
