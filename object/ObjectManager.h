#pragma once
#include <cstdint>

namespace cdc {

class ResolveObject;
struct ObjectBlob;

struct ObjectTracker {
	ResolveObject *resolveObject; // 0
	char *name; // 4
	ObjectBlob *objBlob; // 8
	uint32_t objectListIndex; // C
	uint16_t state; // 10
		// 0 - invalid
		// 1 - valid
		// 2 - valid and allocated
	uint32_t refCount; // 14

	void increaseCountOnResolveObject() {
		// if (resolveObject)
		// 	resolveObject->dword14++;
	}
};

extern ObjectTracker objects[604];

ObjectTracker *getByObjectListIndex(uint32_t objectListIndex);
ObjectTracker *allocObjectSlot(int32_t id, uint16_t state);
void ensureObjectManager();
void readAndParseObjectList();
uint32_t objectIdByName(const char *name);
void requestObject1(uint32_t id);
void requestObject3(uint32_t id);

void buildObjectsUI();

}
