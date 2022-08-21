#include <cstdio>
#include <cstring>
#include "../drm/Resolve.h"
#include "../drm/ResolveObject.h"
#include "../drm/ResolveSection.h"
#include "../filesystem/ArchiveFileSystem.h"
#include "../filesystem/FileHelpers.h"
#include "../game/Gameloop.h"
#include "../game/objects/objects.h"
#include "../object/ObjectManager.h"
#include "../rendering/PCDX11DeviceManager.h"
#include "../spinnycube.h"
#include "../world/SceneLayer.h"
#include "../world/stream.h"
#include "Main.h"

extern HWND hwnd1;

char buildType[16];
char pathPrefix[36];
const char *audioPrefix;
const char *cinematicPrefix;

void *objectiveDatabase;
void *logicActionResourceDatabase;
void *tutorialDatabase;

void SetupBuildDir(const char *prefix) {
	strcpy(buildType, prefix);
	sprintf(pathPrefix, "%s\\", prefix);
	// TODO
}

void buildDRMPath(char *buffer, const char *name) {
	sprintf(buffer, "%s%s.drm", pathPrefix, name);
}


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

void MAIN_DoMainInit() {
	// TODO

	STREAM_Init();

	// TODO

	SceneLayer::init();

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

void MAIN_Init() {

	const char *prefix;
	if (archiveFileSystem_default && archiveFileSystem_default)
		prefix = archiveFileSystem_default->getPrefix();
	else
		prefix = ""; // TODO
	SetupBuildDir(prefix); // WinMain2 already does this
	ensureObjectManager();

	// TODO
	Resolve::Create();
	// TODO
	RegisterGameObjects(); // TODO: call through other function
	// TODO
	MAIN_DoMainInit();
	// TODO

	Gameloop::InitiateLevelLoad("det_sarifhq_rail_tutorial", nullptr);
	getDefaultFileSystem()->processAll();

	// HACK
	spinnyCube(
		hwnd1,
		deviceManager->getD3DDevice(),
		deviceManager->getD3DDeviceContext());
}
