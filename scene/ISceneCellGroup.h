#pragma once

namespace cdc {

class SceneCell;

class ISceneCellGroup {
public:
	// 23 methods
	virtual SceneCell *queryPoint(float *point, bool useThisContainer) = 0; // 14
};

}
