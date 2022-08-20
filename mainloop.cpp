#include <cstdio> // for sprintf
#include "drm/Resolve.h"
#include "drm/ResolveObject.h"
#include "drm/ResolveSection.h"
#include "filesystem/ArchiveFileSystem.h"
#include "filesystem/FileHelpers.h"
#include "game/Gameloop.h"
#include "game/Main.h"
#include "game/objects/objects.h"
#include "object/ObjectManager.h"
#include "rendering/PCDX11DeviceManager.h"
#include "world/SceneLayer.h"
#include "world/stream.h"
#include "mainloop.h"
#include "spinnycube.h"

extern HWND hwnd1;

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

void setups() {
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

void mainloop() {

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
	setups();
	// TODO

	Gameloop::InitiateLevelLoad("det_sarifhq_rail_tutorial", nullptr);
	getDefaultFileSystem()->processAll();

	// HACK
	spinnyCube(
		hwnd1,
		deviceManager->getD3DDevice(),
		deviceManager->getD3DDeviceContext());
}
