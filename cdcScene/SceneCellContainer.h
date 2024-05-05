#pragma once
#include <cstdint>
#include "cdcScene.h"

namespace cdc {

class Scene;
class SceneCell;
class SceneCellGroup;

struct SceneCellBSPNode {
	float x, y, z, dist;
	SceneCellBSPNode *childPos; // index, patched to pointer
	uint32_t cellIndexPos;
	SceneCellBSPNode *childNeg; // index, patched to pointer
	uint32_t cellIndexNeg;
};

struct SceneCellBSP {
	uint32_t nodeCount;
	SceneCellBSPNode *root;

	void assign(uint32_t nodeCount, SceneCellBSPNode *nodes); // substitute indices for pointers
	uint32_t queryPoint(float *point); // look up a cell index
};

class SceneCellContainer {

	friend class SceneCell;

	Scene *scene0;
	SceneCellGroup *sceneCellGroup4;
	SceneCell **cells8 = nullptr;
	uint32_t dwordC = 0;
	SceneCellBSP bsp = {0, nullptr};

public:
	class CellReceiver {
		// 2 methods
	};

	SceneCellContainer(Scene *scene, SceneCellGroup *sceneCellGroup) :
		scene0(scene), sceneCellGroup4(sceneCellGroup) {}

	SceneCell *queryPoint(float *point, bool useThisContainer) {
		// TODO
		return nullptr;
	}

	void assignBSP(uint32_t nodeCount, SceneCellBSPNode *nodes) {
		bsp.assign(nodeCount, nodes);
	}
	void resetBSP() {
		bsp.assign(0, nullptr);
	}
};

}
