#include <cstdio>
#include <cstring>
#include "config.h"
#include "ObjectManager.h"
#include "UIActions.h"
#include "cdcFile/FileHelpers.h"
#include "cdcFile/FileSystem.h"
#include "cdcGameShell/cdcGameShell.h"
#include "rendering/Inspector.h"
#include "cdcResource/ResolveObject.h"

#if ENABLE_IMGUI
#include "../imgui/imgui.h"
#include "cdcResource/ResolveSection.h"
#include "cdcWorld/Object.h"
#include "cdc/dtp/objecttypes/uberobject.h"
#include "UberObject.h"
#endif

#ifndef _WIN32
#define stricmp strcasecmp
#endif

extern char buildType[16];

namespace cdc {

ObjectTracker objects[604];

ObjectManager::ObjectManager() {
	memset(objects, 0, sizeof(objects));
}

ObjectManager *g_objectManager = nullptr;

void ensureObjectManager() {
	if (!g_objectManager)
		g_objectManager = new ObjectManager();
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
	char *filelist = FSHelper_ReadFile(path);

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
	if (g_objectManager->objectList) {
		// TODO
	}

	// example
	// 2
	// 1,foo
	// 2,bar
	//
	// memory layout
	//  0 objectList.count           = 2
	//  4 objectList.byteSize        = 0x20 or more
	//  8 objectList.entries[0].name = &'foo\0'
	//  C objectList.entries[0].slot = 0xffffffff
	// 10 objectList.entries[1].name = &'bar\0'
	// 14 objectList.entries[1].slot = 0xffffffff
	// 18 'foo\0'
	// 1C 'bar\0'

	uint32_t size = 8 + 8 * maxObject + (cursor - filelist);
	auto *allocation = new char[size];
	g_objectManager->objectList = (ObjectList*) allocation;
	char *strings = allocation + (8 + 8 * maxObject);

	g_objectManager->objectList->count = maxObject;
	g_objectManager->objectList->byteSize = size;

	for (int32_t i=0; i<maxObject; i++) {
		g_objectManager->objectList->entries[i].name = 0;
		g_objectManager->objectList->entries[i].slot = ~0u;
	}

	// rewind
	cursor = afterFirstLine;

	for (int32_t i=0; i<numLines; i++) {
		int number;
		cursor = copyLine(buffer, cursor);
		sscanf(buffer, "%d,%s", &number, strings);
		g_objectManager->objectList->entries[number-1].name = strings;
		if (char *comma = strchr(strings, ','))
			*comma = '\0';

		strings += strlen(strings) + 1;
	}

	// auto *e = g_objectManager->objectList->entries;
	// for (int32_t i=0; i<maxObject; i++) {
	// 	printf("%4d %08x %s\n", i, e[i].slot, e[i].name);
	// }

	delete[] filelist;
}

ObjectTracker *getByObjectListIndex(uint32_t objectListIndex) {
	for (uint32_t i = 0; i < 604; i++)
		if (objects[i].state != 0 && objects[i].objectListIndex == objectListIndex)
			return objects + i;

	return nullptr;
}

ObjectTracker *allocObjectSlot(int32_t id, uint16_t state) {
	if (id <= 0 || id >= g_objectManager->objectList->count)
		return nullptr;

	uint32_t i = 0;
	while (i < 604 && objects[i].state) i++;
	if (i == 604)
		return nullptr;

	ObjectTracker *obj = &objects[i];
	obj->state = state;
	obj->objectListIndex = id;
	obj->name = nullptr;
	obj->refCount = 1;
	obj->resolveObject = nullptr;
	obj->debugHide = false;

	g_objectManager->objectList->entries[id-1].slot = i;

	return obj;
}

void objectLoadCallback(void*, void*, void*, ResolveObject* resolveObject) {
	printf("objectLoadCallback %p %s\n", resolveObject, resolveObject->path);
	// TODO
}

void objectUnloadCallback(ObjectTracker*, ResolveObject*) {
	// TODO
}

uint32_t objectIdByName(const char *name) {
	if (int32_t(g_objectManager->objectList->count) <= 0)
		return 0;
	for (int32_t i=0; i < g_objectManager->objectList->count; i++)
		if (stricmp(name, g_objectManager->objectList->entries[i].name) == 0)
			return i+1;
	return 0;
}

static void requestObject(uint32_t id, uint8_t priority) {
	ObjectTracker *objectTracker = getByObjectListIndex(id);

	if (!objectTracker) {
		objectTracker = allocObjectSlot(id, 1);

	} else if (objectTracker->resolveObject) {
		objectTracker->increaseCountOnResolveObject();
		return;
	}

	if (id > g_objectManager->objectList->count)
		readAndParseObjectList();

	char path[256];
	char *objname = g_objectManager->objectList->entries[id-1].name;
	GameShell::LOAD_ObjectFileName(path, objname);
	printf("requesting object %s pobj=%p ", objname, objectTracker);
	objectTracker->resolveObject = ResolveObject::create(
		path,
		&objectLoadCallback,
		nullptr,
		nullptr,
		nullptr,
		&objectUnloadCallback,
		objectTracker,
		0,
		priority);
	printf("robj=%p\n", objectTracker->resolveObject);
}

void requestObjectHigh(uint32_t id) {
	requestObject(id, cdc::FileRequest::HIGH);
}

void requestObjectNormal(uint32_t id) {
	requestObject(id, cdc::FileRequest::NORMAL);
}

char *objectName(uint32_t objectListIndex) {
	return g_objectManager->objectList->entries[objectListIndex-1].name;
}

void buildObjectsUI(UIActions& uiact) {
#if ENABLE_IMGUI
	static bool onlyLoaded = true;
	ImGui::Checkbox("Only show loaded objects", &onlyLoaded);
	if (!g_objectManager || !g_objectManager->objectList) return;
	auto e = g_objectManager->objectList;
	for (int32_t i = 0; i < e->count; i++) {
		bool isLoaded = e->entries[i].slot != ~0u;
		if (e->entries[i].name && (isLoaded || !onlyLoaded)) {
			ImGui::Text("%4d %s %d",
				i+1, e->entries[i].name, e->entries[i].slot);
			ImGui::PushID(i);
			if (!isLoaded) {
				ImGui::SameLine();
				if (ImGui::SmallButton("Load")) {
					requestObjectNormal(i+1);
					getDefaultFileSystem()->processAll();
				}
			} else {
				uint32_t slot = e->entries[i].slot;
				Object *obj = objects[slot].objBlob;
				ImGui::SameLine();
				buildUI(uiact, obj);
			}
			ImGui::PopID();
		}
	}
#endif
}

}

uint32_t buildUI(UIActions& uiact, cdc::Object *obj, Instance *instance) {
	struct ObjProp {
		uint16_t version;
		uint16_t family;
		uint16_t id;
		uint16_t type;
	};
	auto *objProp = (ObjProp*) obj->data;
	uint32_t objFamily = 0;
	if (objProp && objProp->id == 0xb00b)
		objFamily = objProp->family;

#if ENABLE_IMGUI
	ImGui::Text("fam %d/0x%x", objFamily, objFamily);
	ImGui::SameLine();

	bool& hide = cdc::objects[obj->trackerID].debugHide;
	if (hide) {
		if (ImGui::SmallButton("Show")) hide = false;
	} else {
		if (ImGui::SmallButton("Hide")) hide = true;
	}

	if (uint32_t scriptTypeID = obj->dtpData->m_scriptTypeID) {
		ImGui::SameLine();
		if (ImGui::SmallButton("Type")) {
			auto *scriptSection = cdc::g_resolveSections[8];
			if (cdc::ScriptType *scriptType = (cdc::ScriptType*)scriptSection->getWrapped(scriptSection->getDomainId(scriptTypeID)))
				uiact.select(scriptType);
		}
	}

	for (uint32_t j = 0; j < obj->numModels; j++) {
		auto *mesh = obj->models[j]->renderMesh;
		if (!mesh)
			continue;

		ImGui::PushID(j);
		char label[18];
		snprintf(label, 18, "Model %d", j);
		ImGui::SameLine();
		if (ImGui::SmallButton(label))
			uiact.select(mesh);
		ImGui::PopID();
	}

	if (objFamily == 2) {
		auto *uberProp = (dtp::UberObjectProp*) objProp;
		buildUI(uiact, uberProp, instance);
	}

#endif
	return objFamily;
}
