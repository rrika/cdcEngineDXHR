#include "drm/ResolveObject.h"
#include "drm/ResolveSection.h"
#include "drm/sections/DTPDataSection.h"
#include "drm/sections/GenericSection.h"
#include "drm/sections/MaterialSection.h"
#include "drm/sections/ObjectSection.h"
#include "drm/sections/RenderResourceSection.h"
#include "drm/sections/ShaderLibSection.h"
#include "drm/sections/WaveSection.h"
#include "filesystem/ArchiveFileSystem.h"
#include "filesystem/FileHelpers.h"
#include "gameshell/win32/main2.h"
#include "rendering/PCDX11DeviceManager.h"
#include "mainloop.h"
#include "spinnycube.h"
#include "config.h"
#if ENABLE_IMGUI
#include "imgui/imgui.h"
#endif

void setupResolveSections() {
	auto *renderResourceSection = new cdc::RenderResourceSection;

	g_resolveSections[0] = new cdc::GenericSection();
	g_resolveSections[5] = renderResourceSection; // textures
	g_resolveSections[6] = new cdc::WaveSection();
	g_resolveSections[7] = new cdc::DTPDataSection();
	g_resolveSections[9] = new cdc::ShaderLibSection();
	g_resolveSections[10] = new cdc::MaterialSection();
	g_resolveSections[11] = new cdc::ObjectSection();
	g_resolveSections[12] = renderResourceSection; // meshes
}

void *objectiveDatabase;
void *logicActionResourceDatabase;
void *tutorialDatabase;

void loadDatabases() {
	char objectivePath[256];
	char logicActionResourcePath[256];
	char tutorialPath[256];
	
	buildDRMPath(objectivePath, "objective_database");
	cdc::ResolveObject::create(
		objectivePath, nullptr, nullptr, nullptr,
		&objectiveDatabase, nullptr, nullptr, 0, 3);

	buildDRMPath(logicActionResourcePath, "logicactionresource_database");
	cdc::ResolveObject::create(
		logicActionResourcePath, nullptr, nullptr, nullptr,
		&logicActionResourceDatabase, nullptr, nullptr, 0, 3);

	buildDRMPath(tutorialPath, "tutorial_database");
	cdc::ResolveObject::create(
		tutorialPath, nullptr, nullptr, nullptr,
		&tutorialDatabase, nullptr, nullptr, 0, 3);

	getDefaultFileSystem()->processAll();

	uint32_t globalSmartScriptsId = objectIdByName("GlobalSmartScripts");
	requestObject3(globalSmartScriptsId);

	uint32_t globalDatabaseId = objectIdByName("GlobalDatabase");
	requestObject3(globalDatabaseId);

	// this seems to crash
	// uint32_t globalPlayerInfoId = objectIdByName("GlobalPlayerInfo");
	// requestObject3(globalPlayerInfoId);

	uint32_t globalDLCInfoId = objectIdByName("GlobalDLCInfo");
	if (globalDLCInfoId)
		requestObject3(globalDLCInfoId);

	uint32_t globalScriptingId = objectIdByName("GlobalScripting");
	requestObject3(globalScriptingId);

	// TODO
}

extern char buildType[16];

struct ObjListEntry {
	char name[128];
	uint16_t *data;
};

struct ObjList {
	uint32_t count;
	ObjListEntry entries[];
};

ObjList *objList;

void readObjectAndUnitList() {
	// TODO

	// read object names
	readAndParseObjectList();

	// read unit names
	char objListPath[300];
	sprintf(objListPath, "%s\\objlist.dat", buildType);
	if (getDefaultFileSystem()->getSize(objListPath)) {
		objList = (ObjList*) readFileBlocking(objListPath);
		uint32_t count = objList->count;
		ObjListEntry *entry = objList->entries;
		uint16_t *data = (uint16_t*)&objList->entries[objList->count];
		while (count--) {
			if (entry->name[0] != '\0') {
				entry->data = data;
				data += 1 + *data;
			}
			entry++;
		}
	}

	// TODO
}

void buildUnitsUI() {
#if ENABLE_IMGUI
	for (uint32_t i = 0; i < objList->count; i++) {
		ImGui::Text("%s", objList->entries[i].name);
	}
#endif
}

void setups() {
	// TODO

	readObjectAndUnitList();

	// TODO

	uint32_t generalBankId = objectIdByName("generalbank");
	uint32_t globalSoundInfoId = objectIdByName("globalsoundinfo");
	uint32_t globalAnimInfoId = objectIdByName("globalaniminfo");

	requestObject3(generalBankId);
	requestObject3(globalSoundInfoId);
	requestObject3(globalAnimInfoId);

	getDefaultFileSystem()->processAll();

	// TODO

	loadDatabases();

	// TODO
}

void mainloop() {

	const char *prefix;
	if (archiveFileSystem_default && archiveFileSystem_default)
		prefix = archiveFileSystem_default->getPrefix();
	else
		prefix = ""; // TODO
	setupPrefixes(prefix); // WinMain2 already does this
	ensureObjectList();

	// TODO
	setupResolveSections();
	// TODO
	setups();
	// TODO

	// HACK
	spinnyCube(
		hwnd1,
		deviceManager->getD3DDevice(),
		deviceManager->getD3DDeviceContext());
}
