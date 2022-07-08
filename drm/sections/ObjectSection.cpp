#include <cstring>
#include "ObjectSection.h"
#include "../ResolveObject.h"

namespace cdc {

using ObjectBlob = char;

struct PendingObject {
	ResolveObject *resolveObject; // 0
	char *name; // 4
	ObjectBlob *objBlob; // 8
	uint32_t objectListIndex; // C
	uint16_t state; // 10
		// 0 - invalid
		// 1 - valid
		// 2 - valid and allocated
	uint32_t refCount; // 14
};

static PendingObject objects[604];

struct ObjectListEntries {
	uint32_t count; // of entries[]
	uint32_t byteSize; // of the whole ObjectListEntries allocation
	struct Entry {
		char *name;
		uint32_t slot;
	} entries[];
};

struct ObjectList {
	// uint32_t dword00;
	// uint32_t dword04;
	// uint32_t dword08;
	// uint32_t dword0C;
	// uint32_t dword10;
	// ObjectListSub14 *firstSub14;
	ObjectListEntries *objectListEntries = nullptr;
	// uint32_t numNames;
	// ObjectListNamedEntry names[32];
	// uint32_t fun4A0;
	// void *fun4A4_createAppropriateDrawable;
	// void (__cdecl *fun4A8)(GameObject *, GameControl *);

	ObjectList() {
		memset(objects, 0, sizeof(objects));

		// HACK
		uint32_t reverseMapCount = 2000;
		objectListEntries = (ObjectListEntries*) new uint32_t[1 + 2*reverseMapCount];
		objectListEntries->count = reverseMapCount;
	}
};

static ObjectList *g_objectList = nullptr;

void ensureObjectList() {
	if (!g_objectList)
		g_objectList = new ObjectList();
}

static PendingObject *getByObjectListIndex(uint32_t objectListIndex) {
	for (uint32_t i = 0; i < 604; i++)
		if (objects[i].state != 0 && objects[i].objectListIndex == objectListIndex)
			return objects + i;

	return nullptr;
}

static PendingObject *allocObjectSlot(int32_t id, uint16_t state) {
	if (id < 0 || id >= g_objectList->objectListEntries->count)
		return nullptr;

	uint32_t i = 0;
	while (i < 604 && objects[i].state) i++;
	if (i == 604)
		return nullptr;

	PendingObject *obj = &objects[i];
	obj->state = state;
	obj->objectListIndex = id;
	obj->name = nullptr;
	obj->refCount = 1;
	obj->resolveObject = nullptr;

	g_objectList->objectListEntries->entries[id-1].slot = i;

	return obj;
}

uint32_t ObjectSection::realize(uint32_t sectionId, uint32_t unknown6, uint32_t size, bool& alreadyLoaded) {
	auto entry = getByObjectListIndex(sectionId);
	if (!entry) {
		entry = allocObjectSlot(sectionId, 2);

	} else if (entry->state != 1) {
		entry->refCount++;
		alreadyLoaded = true;
		return entry - objects;

	} else {
		entry->state = 2;
	}

	entry->objBlob = new char[size];
	return entry - objects;
}

void ObjectSection::fill(uint32_t domainId, void* src, size_t size, size_t offset) {
	auto &entry = objects[domainId];
	memcpy(entry.objBlob + offset, src, size);
	// return size;
}

void ObjectSection::construct(uint32_t sectionId, void *) {
	// TODO
}

void* ObjectSection::getWrapped(uint32_t domainId) {
	return objects[domainId].objBlob;
}

void* ObjectSection::getBlob(uint32_t domainId) {
	return objects[domainId].objBlob;
}

uint32_t ObjectSection::getDomainId(uint32_t sectionId) {
	for (uint32_t i = 0; i < 604; i++)
		if (objects[i].state && objects[i].objectListIndex == sectionId)
			return i;
	return ~0u;
}

}
