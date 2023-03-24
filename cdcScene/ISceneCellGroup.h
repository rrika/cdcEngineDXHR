#pragma once
#include <cstdint>

namespace cdc {

class Scene;
class SceneCell;

class ISceneCellGroup {
public:
	// 23 methods
	virtual Scene *getScene() = 0; // 4
	virtual uint32_t getCellCount() = 0; // 8
	virtual SceneCell *cellByIndex(uint32_t index) = 0; // 10
	virtual SceneCell *queryPoint(float *point, bool useThisContainer) = 0; // 14
	virtual float *getOrigin() = 0; // 20
	virtual void setUserData(void *) = 0; // 38
	virtual void setName(const char *) = 0; // 48
	virtual const char *getName() = 0; // 4C
};

}
