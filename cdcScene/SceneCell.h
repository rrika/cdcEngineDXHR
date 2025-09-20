#pragma once
#include <cstdlib>
#include <vector>
#include "cdcScene.h"
#include "SceneCellContainer.h"
#include "SceneSubCell.h"

namespace cdc {

class CommonRenderModelInstance;
class SceneCellGroup;
class SceneSubCell;
struct CellData;

class SceneCell :
	public ISceneCell,
	public SceneCellContainer
{
public:
	SceneSubCell *subCells = nullptr;
	uint16_t numSubCells = 0;
	std::vector<SceneCellGroup*> m_childCellGroups; // 28
	CellData *m_pCellData = nullptr; // A8
	CommonRenderModelInstance *renderModelInstance = nullptr; // C8
	const char *m_pName = nullptr; // D0
public:
	SceneCell(SceneCellGroup *group);

	void AddCellData(CellData *pData);

	// 28 methods from ISceneCell
};

}
