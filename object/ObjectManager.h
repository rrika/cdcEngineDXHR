#pragma once
#include <cstdint>

namespace cdc {

class ResolveObject;
struct ObjectBlob;

struct PendingObject { // = cdc::ObjectTracker
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

extern PendingObject objects[604];

PendingObject *getByObjectListIndex(uint32_t objectListIndex);
PendingObject *allocObjectSlot(int32_t id, uint16_t state);
void ensureObjectList();
void readAndParseObjectList();
uint32_t objectIdByName(const char *name);
void requestObject1(uint32_t id);
void requestObject3(uint32_t id);

void buildObjectsUI();

}
