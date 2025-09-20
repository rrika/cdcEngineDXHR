#include "SceneCell.h"
#include "SceneCellGroup.h"
#include "cdcSceneCookdata.h"

namespace cdc {

SceneCell::SceneCell(SceneCellGroup *group) : SceneCellContainer(group->m_pSceneA, group) {
	// TODO
	subCells = new SceneSubCell[1];
	numSubCells = 1;
}

void SceneCell::AddCellData(CellData *pData) {
	// TODO
	m_pCellData = pData;
	m_pName = pData->pHeader->name;

	SceneCellContainer::Init(
		m_pCellGroup->m_cells.data() + pData->pHeader->firstChildIndex,
		pData->pHeader->numChildCells,
		pData->bspNodes,
		pData->pHeader->numBSPNodes);
}

}
