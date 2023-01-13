#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include "game/Main.h"
#include "rendering/BuiltinResources.h"
#include "rendering/IPCDeviceManager.h"
#include "rendering/PCDeviceManager.h"
#include "rendering/PCDX11DeviceManager.h"
#include "rendering/PCDX11RenderDevice.h"
#include "cdcFile/ArchiveFileSystem.h"
#include "cdcFile/FileHelpers.h"
#include "cdcLocale/localstr.h"

using namespace cdc;

HWND hwnd1;
HWND hwnd2;
HWND hwnd3;

bool createWindow() {
	SDL_Window* window = SDL_CreateWindow(
		"Deus Ex: Human Revolution - Director's Cut",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
	if (!window) {
		std::cerr << "Failed to create SDL window" << std::endl;
		return false;
	}

	SDL_CaptureMouse(SDL_TRUE);

	hwnd1 = hwnd2 = hwnd3 = window;
	return true;
}

int main(int argc, char** argv) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		std::cerr << "Failed to init SDL" << std::endl;
		return 1;
	}

	loadBuiltinResources();
	createLowerFileSystems();
	createHigherFileSystems();
	if (archiveFileSystem_default) {
		SetupBuildDir(archiveFileSystem_default->getPrefix());
	}
	localstr_reload(); // HACK

	auto deviceManager = createPCDeviceManager();
	if (!createWindow())
		return 1;
	cdc::DisplayConfig *displayConfig = deviceManager->getDisplayConfig();
	displayConfig->fullscreenRefreshRate = 60;
	displayConfig->fullscreen = false;
	displayConfig->fullscreenWidth = 1366;
	displayConfig->fullscreenHeight = 768;
	displayConfig->sampleCount = 1;
	displayConfig->sampleQuality = 0;
	displayConfig->enableTripleBuffer = false;
	displayConfig->enableVsync = true;
	displayConfig->lockWindowResolution = false;
	g_renderDevice = createPCDX11RenderDevice(hwnd1, 640, 480, 0);

	MAIN_Init();
	return 0;
}
