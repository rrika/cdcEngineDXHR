#include <cstdio>
#include <cstring>
#include "config.h"
#include "ObjectManager.h"
#include "UIActions.h"
#include "cdcFile/FileHelpers.h"
#include "cdcFile/FileSystem.h"
#include "cdcGameShell/cdcGameShell.h"
#include "cdcRender/Inspector.h"
#include "cdcResource/Resolve.h"
#include "cdcResource/ResolveObject.h"
#include "game/dtp/modularhuman.h"

#if ENABLE_IMGUI
#include "../imgui/imgui.h"
#include "cdcResource/ResolveSection.h"
#include "cdcWorld/Object.h"
#endif

#ifndef _WIN32
#define stricmp strcasecmp
#endif

extern char buildType[16];

namespace cdc {

void ObjectTracker::increaseCountOnResolveObject() {
	if (resolveObject)
		resolveObject->m_ref++;
}

ObjectTracker objects[604];

ObjectManager::ObjectManager() {
	memset(objects, 0, sizeof(objects));
}

void ObjectManager::RegisterObjectFamily(
	int32_t fId,
	InitFunc *fInit,
	InitFunc *fUninit,
	ProcessFunc *fProcess,
	CollideFunc *fCollide,
	CollideFunc *fAdditionalCollide,
	QueryFunc *fQuery,
	MessageFunc *fMessage,
	DebugDrawFunc *fDebugDraw,
	void *unknown)
{
	// TODO
	m_familyMap[fId] = {
		fInit,
		fUninit,
		fProcess,
		fCollide,
		fAdditionalCollide,
		fQuery,
		fMessage,
		fDebugDraw,
		unknown
	};
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

	g_objectManager->debugLoad = new bool[maxObject+1];

	uint32_t size = 8 + 8 * maxObject + (cursor - filelist);
	auto *allocation = new char[size];
	g_objectManager->objectList = (ObjectList*) allocation;
	char *strings = allocation + (8 + 8 * maxObject);

	g_objectManager->objectList->count = maxObject;
	g_objectManager->objectList->byteSize = size;

	for (int32_t i=0; i<maxObject; i++) {
		g_objectManager->objectList->entries[i].name = 0;
		g_objectManager->objectList->entries[i].slot = ~0u;
		g_objectManager->debugLoad[i] = false;
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

void objectCancelCallback(ObjectTracker *ot, ResolveObject *ro) {
	if (ot->objBlob) {
		g_objectManager->objectList->entries[ot->objectListIndex-1].slot = ~0u;
		ot->resolveObject = nullptr;
		ot->state = 0;
	}
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
		objectTracker->increaseCountOnResolveObject(); // increase m_ref
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
		&objectCancelCallback,
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

void releaseObject(uint32_t id) {
	// HACK
	if (ObjectTracker *otr = getByObjectListIndex(id))
		if (otr->resolveObject)
			Resolve::Release(otr->resolveObject);
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
			if (g_objectManager->debugLoad[i] == false) {
				ImGui::SameLine();
				if (ImGui::SmallButton(isLoaded ? "Pin" : "Load")) {
					g_objectManager->debugLoad[i] = true;
					requestObjectNormal(i+1);
					getDefaultFileSystem()->processAll();
				}
			}
			if (isLoaded) {
				uint32_t slot = e->entries[i].slot;
				Object *obj = objects[slot].objBlob;
				ImGui::SameLine();
				if (auto ro = objects[slot].resolveObject) {
					uint32_t refCount = ro->m_ref;
					ImGui::Text("(ref=%d)", ro->m_ref);
					if (g_objectManager->debugLoad[i] == true) {
						ImGui::SameLine();
						if (ImGui::SmallButton("Unload")) {
							// TODO: implement a helper for this
							objects[slot].resolveObject->Release();
							g_objectManager->debugLoad[i] = false;
							if (refCount == 1)
								obj = nullptr;
						}
					}
				} else {
					ImGui::Text("(no object)");
				}
				if (obj) {
					ImGui::SameLine();
					buildUI(uiact, obj);
				}
			}
			ImGui::PopID();
		}
	}
#endif
}

}

uint32_t buildUI(UIActions& uiact, cdc::Object *obj) {
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
	ImGui::PushID(obj);
	ImGui::Text("fam %d/0x%x", objFamily, objFamily);
	ImGui::SameLine();

	bool& hide = cdc::objects[obj->trackerID].debugHide;
	if (hide) {
		if (ImGui::SmallButton("Show")) hide = false;
	} else {
		if (ImGui::SmallButton("Hide")) hide = true;
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

	if (uint32_t scriptTypeID = obj->dtpData->m_scriptTypeID) {
		ImGui::SameLine();
		char label[20];
		snprintf(label, 20, "Type %x", scriptTypeID);
		if (ImGui::SmallButton(label)) {
			auto *scriptSection = cdc::g_resolveSections[8];
			if (cdc::ScriptType *scriptType = (cdc::ScriptType*)scriptSection->GetBasePointer(scriptSection->FindResource(scriptTypeID)))
				uiact.select(scriptType);
		}
	}

	if (objFamily == 93) {
		auto *modular = *(dtp::ModularHuman**)(0x48 + (char*)objProp);
		if (!modular) modular = (dtp::ModularHuman*)(0x8 + (char*)objProp);
		ImGui::Indent();
		ImGui::Text("Head");
		if (modular->material8) {
			ImGui::SameLine();
			if (ImGui::SmallButton("Material 8"))
				uiact.select(modular->material18);
		}
		if (modular->materialC) {
			ImGui::SameLine();
			if (ImGui::SmallButton("Material C"))
				uiact.select(modular->materialC);
		}
		if (modular->upperBody) {
			if (ImGui::SmallButton("Upper body"))
				uiact.select(modular->upperBody);
			if (modular->material18) {
				ImGui::SameLine();
				if (ImGui::SmallButton("Material 18"))
					uiact.select(modular->material18);
			}
			if (modular->material1C) {
				ImGui::SameLine();
				if (ImGui::SmallButton("Material 1C"))
					uiact.select(modular->material1C);
			}
		}
		if (modular->hands) {
			if (ImGui::SmallButton("Hands"))
				uiact.select(modular->hands);
			if (modular->material18) {
				ImGui::SameLine();
				if (ImGui::SmallButton("Material 24"))
					uiact.select(modular->material24);
			}
		}
		if (modular->lowerBody) {
			if (ImGui::SmallButton("Lower body"))
				uiact.select(modular->lowerBody);
			if (modular->material2C) {
				ImGui::SameLine();
				if (ImGui::SmallButton("Material 2C"))
					uiact.select(modular->material2C);
			}
			if (modular->material30) {
				ImGui::SameLine();
				if (ImGui::SmallButton("Material 30"))
					uiact.select(modular->material30);
			}
			if (modular->material34) {
				ImGui::SameLine();
				if (ImGui::SmallButton("Material 34"))
					uiact.select(modular->material34);
			}
		}
		ImGui::Unindent();
	}

	ImGui::PopID();
#endif
	return objFamily;
}
