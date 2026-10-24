#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_vulkan.h>
#include "../../game/Main.h"
#include "cdcRender/BuiltinResources.h"
#include "cdcRender/IPCDeviceManager.h"
#include "cdcRender/PCDX11DeviceManager.h"
#include "cdcRender/PCDX11RenderDevice.h"
#include "cdcFile/ArchiveFileSystem.h"
#include "cdcFile/FileHelpers.h"
#include "cdcLocale/localstr.h"
#include "cdcResource/Specialisation.h"
#include "cdcWorld/RenderLayer.h"

using namespace cdc;

HWND hwnd1;
HWND hwnd2;
HWND hwnd3;

bool createWindow() {
	SDL_Window* window = SDL_CreateWindow(
		"Deus Ex: Human Revolution - Director's Cut",
		640, 480,
		SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
	if (!window) {
		std::cerr << "Failed to create SDL window" << std::endl;
		return false;
	}

	SDL_CaptureMouse(true);

	hwnd1 = hwnd2 = hwnd3 = window;
	return true;
}

int main(int argc, char** argv) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == false) {
		std::cerr << "Failed to init SDL" << std::endl;
		return 1;
	}
	setenv("DXVK_WSI_DRIVER", "SDL3", 1);

	loadBuiltinResources();
	createLowerFileSystems();
	createHigherFileSystems();
	if (archiveFileSystem_default) {
		SetupBuildDir(archiveFileSystem_default->getPrefix());
	}
	localstr_reload(); // HACK

	auto deviceManager = createPCDX11DeviceManager();
	if (!createWindow())
		return 1;
	cdc::DisplayConfig *displayConfig = deviceManager->getDisplayConfig();
	displayConfig->fullscreenWidth = 1366;
	displayConfig->fullscreenHeight = 768;
	displayConfig->fullscreenRefreshRate = 60;
	displayConfig->fullscreen = false;
	displayConfig->lockWindowResolution = false;
	displayConfig->enableVsync = true;
	displayConfig->enableTripleBuffer = false;
	displayConfig->antiAliasing = 0;
	displayConfig->sampleCount = 1;
	displayConfig->sampleQuality = 0;
	g_renderDevice = createPCDX11RenderDevice(hwnd1, 640, 480, 0);

	{
		FileSystem *fs = getDefaultFileSystem();
		uint32_t mask = fs->getLanguageMask();
		mask &= 0x3fffffff;
		mask |= 0x80000000; // dx11
		Specialisation::BlockingChange(mask);
	}

	MainG2();

	cdcRenderLayer::Destroy();

	SDL_Quit();

	return 0;
}
