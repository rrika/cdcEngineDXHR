#include "drm/sections/DTPDataSection.h"
#include "drm/sections/GenericSection.h"
#include "drm/sections/MaterialSection.h"
#include "drm/sections/RenderResourceSection.h"
#include "drm/sections/ShaderLibSection.h"
#include "drm/sections/WaveSection.h"
#include "filesystem/ArchiveFileSystem.h"
#include "rendering/PCDX11DeviceManager.h"
#include "main2.h"
#include "mainloop.h"
#include "spinnycube.h"

cdc::ResolveSection *resolveSections[16] = {nullptr};

void setupResolveSections() {
	auto *renderResourceSection = new cdc::RenderResourceSection;

	resolveSections[0] = new cdc::GenericSection();
	resolveSections[5] = renderResourceSection; // textures
	resolveSections[6] = new cdc::WaveSection();
	resolveSections[7] = new cdc::DTPDataSection();
	resolveSections[9] = new cdc::ShaderLibSection();
	resolveSections[10] = new cdc::MaterialSection();
	resolveSections[12] = renderResourceSection; // meshes
}

void setups() {
	// TODO
}

void mainloop() {

	const char *prefix;
	if (archiveFileSystem_default && archiveFileSystem_default)
		prefix = archiveFileSystem_default->getPrefix();
	else
		prefix = ""; // TODO
	setupPrefixes(prefix); // WinMain2 already does this

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
