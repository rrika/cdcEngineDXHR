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
#include "rendering/PCDX11DeviceManager.h"
#include "main2.h"
#include "mainloop.h"
#include "spinnycube.h"

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

void setups() {
	// TODO

	readAndParseObjectList(); // called indirectly

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
