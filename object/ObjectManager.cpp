#include <cstdio>
#include <cstring>
#include "ObjectManager.h"
#include "../drm/ResolveObject.h"
#include "../../filesystem/FileHelpers.h"

void buildDRMPath(char *buffer, const char *name);
extern char buildType[16];

namespace cdc {

PendingObject objects[604];

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
	}
};

static ObjectList *g_objectList = nullptr;

void ensureObjectList() {
	if (!g_objectList)
		g_objectList = new ObjectList();
}

static char *copyLine(char *dst, char *src) {
	if (*src != '\n') {
		while (*src != '\r') {
			*dst++ = *src++;
			if (*src == '\n') {
				src++;
				break;
			}
		}
	}
	while (*src == '\n' || *src == '\r')
		src++;
	*dst = '\0';
	return src;
}

void readAndParseObjectList() {
	char path[80];
	sprintf(path, "%s\\objectlist.txt", buildType);
	char *filelist = readFileBlocking(path);

	int numLines; 

	char buffer[512];
	auto *cursor = copyLine(buffer, filelist);
	auto *afterFirstLine = cursor;
	sscanf(buffer, "%d", &numLines);

	int maxObject = 0;

	for (int32_t i=0; i<numLines; i++) {
		int number;
		cursor = copyLine(buffer, cursor);
		sscanf(buffer, "%d,", &number);
		if (number > maxObject)
			maxObject = number;
	}

	// have to reallocate
	if (g_objectList->objectListEntries) {
		// TODO
	}

	// example
	// 2
	// 1,foo
	// 2,bar
	//
	// memory layout
	//  0 objectListEntries.count           = 2
	//  4 objectListEntries.byteSize        = 0x20 or more
	//  8 objectListEntries.entries[0].name = &'foo\0'
	//  C objectListEntries.entries[0].slot = 0xffffffff
	// 10 objectListEntries.entries[1].name = &'bar\0'
	// 14 objectListEntries.entries[1].slot = 0xffffffff
	// 18 'foo\0'
	// 1C 'bar\0'

	uint32_t size = 8 + 8 * maxObject + (cursor - filelist);
	auto *allocation = new char[size];
	g_objectList->objectListEntries = (ObjectListEntries*) allocation;
	char *strings = allocation + (8 + 8 * maxObject);

	g_objectList->objectListEntries->count = maxObject;
	g_objectList->objectListEntries->byteSize = size;

	for (int32_t i=0; i<maxObject; i++) {
		g_objectList->objectListEntries->entries[i].name = 0;
		g_objectList->objectListEntries->entries[i].slot = ~0u;
	}

	// rewind
	cursor = afterFirstLine;

	for (int32_t i=0; i<numLines; i++) {
		int number;
		cursor = copyLine(buffer, cursor);
		sscanf(buffer, "%d,%s", &number, strings);
		g_objectList->objectListEntries->entries[number-1].name = strings;
		if (char *comma = strchr(strings, ','))
			*comma = '\0';

		strings += strlen(strings) + 1;
	}

	// auto *e = g_objectList->objectListEntries->entries;
	// for (int32_t i=0; i<maxObject; i++) {
	// 	printf("%4d %08x %s\n", i, e[i].slot, e[i].name);
	// }

	delete[] filelist;
}

PendingObject *getByObjectListIndex(uint32_t objectListIndex) {
	for (uint32_t i = 0; i < 604; i++)
		if (objects[i].state != 0 && objects[i].objectListIndex == objectListIndex)
			return objects + i;

	return nullptr;
}

PendingObject *allocObjectSlot(int32_t id, uint16_t state) {
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

void objectLoadCallback(void*, void*, void*, ResolveObject* resolveObject) {
	printf("objectLoadCallback %p %s\n", resolveObject, resolveObject->path);
	// TODO
}

void objectUnloadCallback(PendingObject*, ResolveObject*) {
	// TODO
}

uint32_t objectIdByName(const char *name) {
	if (int32_t(g_objectList->objectListEntries->count) <= 0)
		return 0;
	for (int32_t i=0; i < g_objectList->objectListEntries->count; i++)
		if (stricmp(name, g_objectList->objectListEntries->entries[i].name) == 0)
			return i+1;
	return 0;
}

static void requestObject(uint32_t id, uint8_t fsMethod18Arg) {
	PendingObject *pendingObject = getByObjectListIndex(id);

	if (!pendingObject) {
		pendingObject = allocObjectSlot(id, 1);

	} else if (pendingObject->resolveObject) {
		pendingObject->increaseCountOnResolveObject();
		return;
	}

	if (id > g_objectList->objectListEntries->count)
		readAndParseObjectList();

	char path[256];
	char *objname = g_objectList->objectListEntries->entries[id-1].name;
	buildDRMPath(path, objname);
	printf("requesting object %s pobj=%p ", objname, pendingObject);
	pendingObject->resolveObject = ResolveObject::create(
		path,
		&objectLoadCallback,
		nullptr,
		nullptr,
		nullptr,
		&objectUnloadCallback,
		pendingObject,
		0,
		fsMethod18Arg);
	printf("robj=%p\n", pendingObject->resolveObject);
}

void requestObject1(uint32_t id) {
	requestObject(id, 1);
}

void requestObject3(uint32_t id) {
	requestObject(id, 3);
}

void buildObjectsUI() {
#if ENABLE_IMGUI
	static bool onlyLoaded = true;
	ImGui::Checkbox("Only show loaded objects", &onlyLoaded);
	if (!g_objectList || !g_objectList->objectListEntries) return;
	auto e = g_objectList->objectListEntries;
	for (int32_t i = 0; i < e->count; i++) {
		bool isLoaded = e->entries[i].slot != ~0u;
		if (e->entries[i].name && (isLoaded || !onlyLoaded)) {
			ImGui::Text("%4d %s %d",
				i+1, e->entries[i].name, e->entries[i].slot);
			if (!isLoaded) {
				ImGui::PushID(i);
				ImGui::SameLine();
				if (ImGui::SmallButton("Load")) {
					requestObject3(i+1);
					getDefaultFileSystem()->processAll();
				}
				ImGui::PopID();
			}
		}
	}
#endif
}

}
