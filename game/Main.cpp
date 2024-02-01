#include <cstdio>
#include <cstring>
#include "cdcFile/ArchiveFileSystem.h"
#include "cdcFile/FileHelpers.h"
#include "game/Gameloop.h"
#include "game/objects/objects.h"
#include "game/ui/ui.h"
#include "cdcGameShell/cdcGameShell.h"
#include "cdcObjects/ObjectManager.h"
#include "../rendering/PCDX11DeviceManager.h"
#include "postprocessing/PPManager.h"
#include "cdcResource/Resolve.h"
#include "cdcResource/ResolveObject.h"
#include "cdcResource/ResolveSection.h"
#include "../spinnycube.h"
#include "cdcScript/GarbageCollector.h"
#include "cdcScript/ScriptManager.h"
#include "cdcSound/Sound.h"
#include "cdcSound/MultiplexStream.h"
#include "cdcWorld/Object.h"
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
	char streamdir[256];
	char cinstreamdir[256];
	strcpy(buildType, prefix);
	sprintf(pathPrefix, "%s\\", prefix);
	sprintf(streamdir, "%saudio\\streams\\", pathPrefix);
	sprintf(cinstreamdir, "%sart\\cinematics\\cinstream\\", pathPrefix);
	MultiplexStream::SetSoundDirectory(streamdir);
	MultiplexStream::SetCinematicDirectory(cinstreamdir);
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

	getDefaultFileSystem()->processAll();

	// TODO

	ObjectTracker *ort_globalDatabase = getByObjectListIndex(globalDatabaseId);
	globalDatabase = (GlobalDatabase*)ort_globalDatabase->objBlob->data;
}

void Init_NativeScripts();

void MAIN_DoMainInit() {
	// TODO

	GarbageCollector::Create();
	ScriptManager::s_instance = new ScriptManager();
	Init_NativeScripts();

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

	UI_Begin();

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

	PPManager::s_instance = new PPManager();

	// Gameloop::InitiateLevelLoad("det_sarifhq_rail_tutorial", nullptr);
	getDefaultFileSystem()->processAll();

	// HACK
	spinnyCube(
		hwnd1,
		deviceManager->getD3DDevice(),
		deviceManager->getD3DDeviceContext());
}
