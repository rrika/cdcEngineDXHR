#include "SceneCellContainer.h"
#include "SceneCellGroup.h"

namespace cdc {

SceneCell *SceneCellContainer::GetCellFromPoint(Vector3 const& point, bool ignoreChildCellGroups) {

	if (ignoreChildCellGroups == false && this != static_cast<SceneCellContainer*>(m_pCellGroup)) {
		auto *thisCell = static_cast<SceneCell*>(this);
		for (auto subCellGroup : thisCell->m_childCellGroups)
			if (auto cell = subCellGroup->GetCellFromPoint(point, ignoreChildCellGroups))
				return cell;

		if (m_numChildren == 0)
			return thisCell;

		// else fall through to BSP query
	}

	if (m_bsp.m_numNodes == 0)
		return m_pChildren[0];

	Vector3 localPoint {point +- *m_pCellGroup->getOrigin()};
	uint32_t cellIndex = cdc::GetCellFromPoint(m_bsp, point);
	if (cellIndex == uint32_t(-1))
		return nullptr;

	SceneCell *childCell = m_pChildren[cellIndex];
	if (childCell->m_numChildren > 0 || (childCell->m_childCellGroups.size() != 0 && ignoreChildCellGroups == false)) 
		if (auto childResultCell = childCell->GetCellFromPoint(point, ignoreChildCellGroups))
			return childResultCell;

	return childCell;
}

}
