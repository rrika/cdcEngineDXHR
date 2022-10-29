#include <cstdio>
#include <cstring>
#include "../drm/Resolve.h"
#include "../drm/ResolveObject.h"
#include "../drm/ResolveSection.h"
#include "cdcFile/ArchiveFileSystem.h"
#include "cdcFile/FileHelpers.h"
#include "../game/Gameloop.h"
#include "../game/objects/objects.h"
#include "../gameshell/cdcGameShell.h"
#include "cdcObjects/ObjectManager.h"
#include "../rendering/PCDX11DeviceManager.h"
#include "../spinnycube.h"
#include "cdcSound/Sound.h"
#include "cdcWorld/SceneLayer.h"
#include "cdcWorld/stream.h"
#include "Main.h"

using namespace cdc;

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

void loadDatabases() {
	char objectivePath[256];
	char logicActionResourcePath[256];
	char tutorialPath[256];
	
	GameShell::LOAD_ObjectFileName(objectivePath, "objective_database");
	ResolveObject::create(
		objectivePath, nullptr, nullptr, nullptr,
		&objectiveDatabase, nullptr, nullptr, 0, cdc::FileRequest::NORMAL);

	GameShell::LOAD_ObjectFileName(logicActionResourcePath, "logicactionresource_database");
	ResolveObject::create(
		logicActionResourcePath, nullptr, nullptr, nullptr,
		&logicActionResourceDatabase, nullptr, nullptr, 0, cdc::FileRequest::NORMAL);

	GameShell::LOAD_ObjectFileName(tutorialPath, "tutorial_database");
	ResolveObject::create(
		tutorialPath, nullptr, nullptr, nullptr,
		&tutorialDatabase, nullptr, nullptr, 0, cdc::FileRequest::NORMAL);

	getDefaultFileSystem()->processAll();

	uint32_t globalSmartScriptsId = objectIdByName("GlobalSmartScripts");
	requestObjectNormal(globalSmartScriptsId);

	uint32_t globalDatabaseId = objectIdByName("GlobalDatabase");
	requestObjectNormal(globalDatabaseId);

	// this seems to crash
	// uint32_t globalPlayerInfoId = objectIdByName("GlobalPlayerInfo");
	// requestObjectNormal(globalPlayerInfoId);

	uint32_t globalDLCInfoId = objectIdByName("GlobalDLCInfo");
	if (globalDLCInfoId)
		requestObjectNormal(globalDLCInfoId);

	uint32_t globalScriptingId = objectIdByName("GlobalScripting");
	requestObjectNormal(globalScriptingId);

	// TODO
}

void MAIN_DoMainInit() {
	// TODO

	STREAM_Init();

	// TODO

	SOUND_Init();

	// TODO

	SceneLayer::init();

	// TODO

	uint32_t generalBankId = objectIdByName("generalbank");
	uint32_t globalSoundInfoId = objectIdByName("globalsoundinfo");
	uint32_t globalAnimInfoId = objectIdByName("globalaniminfo");

	requestObjectNormal(generalBankId);
	requestObjectNormal(globalSoundInfoId);
	requestObjectNormal(globalAnimInfoId);

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

	// Gameloop::InitiateLevelLoad("det_sarifhq_rail_tutorial", nullptr);
	getDefaultFileSystem()->processAll();

	// HACK
	spinnyCube(
		hwnd1,
		deviceManager->getD3DDevice(),
		deviceManager->getD3DDeviceContext());
}
