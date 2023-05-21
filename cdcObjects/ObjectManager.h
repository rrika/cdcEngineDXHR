#pragma once
#include <cstdint>
#include "ObjectIncludes.h"

struct UIActions;

namespace cdc {

class ResolveObject;
struct Object;

struct ObjectTracker {
	ResolveObject *resolveObject; // 0
	char *name; // 4
	Object *objBlob; // 8
	uint32_t objectListIndex; // C
	uint16_t state; // 10
		// 0 - invalid
		// 1 - valid
		// 2 - valid and allocated
	uint32_t refCount; // 14

	bool debugHide; // HACK

	void increaseCountOnResolveObject() {
		// if (resolveObject)
		// 	resolveObject->dword14++;
	}
};

extern ObjectTracker objects[604];

struct ObjectList {
	uint32_t count; // of entries[]
	uint32_t byteSize; // of the whole ObjectList allocation
	struct Entry {
		char *name;
		uint32_t slot;
	} entries[];
};

struct ObjectManager {
	// uint32_t dword00;
	// uint32_t dword04;
	// uint32_t dword08;
	// uint32_t dword0C;
	// uint32_t dword10;
	// ObjectManagerSub14 *firstSub14;
	ObjectList *objectList = nullptr;
	// uint32_t numNames;
	// ObjectManagerNamedEntry names[32];
	InitFunc *preInit; // 4A0, GameAdditionalPreInit
	InitFunc *postInit; // 4A4, GameAdditionalPostInit, create drawable
	InitFunc *uninit; // 4A8, GameAdditionalUninit

	ObjectManager();
};

extern ObjectManager *g_objectManager;

ObjectTracker *getByObjectListIndex(uint32_t objectListIndex);
ObjectTracker *allocObjectSlot(int32_t id, uint16_t state);
void ensureObjectManager();
void readAndParseObjectList();
uint32_t objectIdByName(const char *name);
void requestObjectHigh(uint32_t id);
void requestObjectNormal(uint32_t id);

void buildObjectsUI(UIActions& uiact);

}
