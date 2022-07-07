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

void setups() {
	// TODO
	readAndParseObjectList(); // called indirectly

	uint32_t generalBankId = objectIdByName("generalbank");
	uint32_t globalSoundInfoId = objectIdByName("globalsoundinfo");
	uint32_t globalAnimInfoId = objectIdByName("globalaniminfo");

	requestObject3(generalBankId);
	requestObject3(globalSoundInfoId);
	requestObject3(globalAnimInfoId);

	getDefaultFileSystem()->processAll();
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
