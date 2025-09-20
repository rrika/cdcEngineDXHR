#pragma once
#include <cstdint>
#include "cdcScene.h"
#include "CookedBSP.h"

namespace cdc {

class Scene;
class SceneCell;
class SceneCellGroup;

class SceneCellContainer {
public:
	Scene *m_pSceneA; // 0, as opposed to m_pSceneB in derived classes
	SceneCellGroup *m_pCellGroup;
	SceneCell **m_pChildren = nullptr;
	uint32_t m_numChildren = 0; // C
	CookedBSP m_bsp;

public:
	class CellReceiver {
		// 2 methods
	};

	SceneCellContainer(Scene *scene, SceneCellGroup *sceneCellGroup) :
		m_pSceneA(scene), m_pCellGroup(sceneCellGroup) {}

	SceneCell *GetCellFromPoint(Vector3 const& point, bool ignoreChildCellGroups);

	void Init(SceneCell **cells, uint32_t numChildren, void *nodes, uint32_t nodeCount) {
		m_pChildren = cells;
		m_numChildren = numChildren;
		m_bsp.Init(nodeCount, nodes);
	}

	void Uninit() {
		m_bsp.Init(0, nullptr);
	}
};

}
