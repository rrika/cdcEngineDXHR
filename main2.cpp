#include <cstdio>
#include <windows.h>
#include "filesystem/ArchiveFileSystem.h"
#include "filesystem/FileUserBufferReceiver.h"
#include "filesystem/HackFileSystem.h"
#include "imgui/imgui.h"
#include "main.h"
#include "main2.h"
#include "mainloop.h"
#include "rendering/BuiltinResources.h"
#include "rendering/IPCDeviceManager.h"
#include "rendering/PCDX11DeviceManager.h"
#include "rendering/PCDX11RenderDevice.h"
#include "sys/Assert.h"

using namespace cdc;

DisplayConfig g_displayConfig;

void initDisplayConfig() {
	// TODO
	g_displayConfig.fullscreenRefreshRate = 60;
	g_displayConfig.fullscreen = false;
	g_displayConfig.fullscreenWidth = 1366;
	g_displayConfig.fullscreenHeight = 768;
	g_displayConfig.sampleCount = 1;
	g_displayConfig.sampleQuality = 0;
	g_displayConfig.enableTripleBuffer = false;
	g_displayConfig.enableVsync = true;
	g_displayConfig.lockWindowResolution = false;
}

void initDisplayConfigFromDisplay() {
	// TODO
}

FileSystem *lowerFileSystem;
FileSystem *threadedLowerFileSystem;

void createLowerFileSystems() {
	// TODO
	lowerFileSystem = new HackFileSystem();
	threadedLowerFileSystem = lowerFileSystem;
}

ArchiveFileSystem *archiveFileSystem_default;

void createHigherFileSystems() {
	const char *bigfilePath = getenv("BIGFILE");
	if (!bigfilePath) {
		printf("\nspecify path to BIGFILE.000 through BIGFILE environment variable\n\n");
		return;
	}

	archiveFileSystem_default = new ArchiveFileSystem(lowerFileSystem);
	bool indexOk = archiveFileSystem_default->readIndex(bigfilePath, 0);
	
	if (!indexOk)
		FatalError("Unable to open bigfile BIGFILE.DAT!");
}

void destroyFileSystems() {
	// TODO
}

FileSystem *getDefaultFileSystem() {
	// TODO
	return archiveFileSystem_default;
}

char *readFileBlocking(const char *path) {
	auto fs = archiveFileSystem_default; // HACK
	uint32_t size = fs->getSize(path);

	char *buffer = new char[size+1];
	FileReceiver *rec = FileUserBufferReceiver::create(buffer);
	FileRequest *req = fs->createRequest(rec, path, 0);
	req->submit(1);
	fs->processUntil(req);
	req->decrRefCount();
	// req is owned by fs which takes care of it in processAll()

	return buffer;
}

char buildType[16];
char pathPrefix[36];
const char *audioPrefix;
const char *cinematicPrefix;

void setupPrefixes(const char *prefix) {
	strcpy(buildType, prefix);
	sprintf(pathPrefix, "%s\\", prefix);
	// TODO
}

void buildDRMPath(char *buffer, const char *name) {
	sprintf(buffer, "%s%s.drm", pathPrefix, name);
}

bool hasSSE;
bool hasMMX;
bool haveDX9Device;
bool haveDX11Device;

bool createDeviceManager() {
	hasSSE = true; // TODO
	hasMMX = true; // TODO

	haveDX9Device = false; // TODO
	haveDX11Device = createPCDX11DeviceManager() != nullptr;

	return haveDX9Device || haveDX11Device;
}

IPCDeviceManager *deviceManager9 = nullptr;
uint32_t useDX11 = 1;

IPCDeviceManager *getDeviceManager() {
	if (useDX11 == 0)
		return deviceManager9;
	if (useDX11 == 1)
		return deviceManager;
	return nullptr;
}

bool showSetup() {
	// TODO
	return true;
}

bool showSetupIfNecessary(uint32_t unknown, uint32_t showSetupOrConfig) {
	if (getDeviceManager()->isConfigAcceptable(&g_displayConfig)) {
		if (showSetupOrConfig == 0) {
			// registrySetTopmostAndForceFeedback();
			return true;
		}
	} else {
		initDisplayConfigFromDisplay();
	}
	return showSetup(); // TODO
}

#if ENABLE_IMGUI
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT CALLBACK gameWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

#if ENABLE_IMGUI
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;
#endif

	// if (uMsg == WM_DEUS_EX_GCS_FEEDBACK) { /*TODO*/ }

	switch (uMsg)
	{
	case WM_CHAR:
		// TODO
		break;
	case WM_SIZE:
		// TODO
		break;
	case WM_ACTIVATE:
		// TODO
		break;
	case WM_KEYUP:
		// TODO
		break;
	case WM_PAINT:
		// TODO
		break;
	case WM_CLOSE:
		// TODO
		break;
	case WM_MOVE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		if (g_renderDevice)
			static_cast<PCDX11RenderDevice*>(g_renderDevice)->handleResize(rect.right, rect.bottom);
		// TODO
		break;
	}
	case 0xBB8:
		// TODO
		break;
	case 0x8000:
		// TODO
		break;
	case WM_SIZING:
		// TODO
		break;
	case WM_SYSKEYDOWN:
		// TODO
		break;
	case WM_SYSCOMMAND:
		// TODO
		break;
	case WM_MOUSEFIRST:
		// TODO
		break;
	case WM_DESTROY: // hack
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

DisplayConfig displayConfig;
HWND hwnd1;
HWND hwnd2;
HWND hwnd3;
void *yellowCursor;

void createWindow() {
	WNDCLASSW wndClass = { sizeof(wndClass) };
	wndClass.lpfnWndProc = (WNDPROC)gameWndProc;
	wndClass.hInstance = GetModuleHandleW(0);
	wndClass.hIcon = LoadIconW(wndClass.hInstance, (LPCWSTR)0x66);
	// if ( !wndClass.hIcon )
	// 	wndClass.hIcon = LoadIconW(0, (LPCWSTR)0x7F00);
	wndClass.lpszClassName = L"DeusExHRDCE";
	// wndClass.hbrBackground = (HBRUSH)GetStockObject(5);
	wndClass.hCursor = LoadCursorW(0, (LPCWSTR)0x7F00); // normal cursor

	if (true) { // HACK
		ICONINFO ii;
		auto pointerIcon = LoadIconA(wndClass.hInstance, (LPCSTR)109);
		GetIconInfo(pointerIcon, &ii);
		ii.fIcon = 0;
		ii.xHotspot = 5;
		ii.yHotspot = 5;
		wndClass.hCursor = CreateIconIndirect(&ii); // new and improved (yellow) cursor
	}

	yellowCursor = wndClass.hCursor;

	RegisterClassW(&wndClass);

	wchar_t windowName[] = L"Deus Ex: Human Revolution - Director's Cut";

	hwnd1 = hwnd2 = hwnd3 = CreateWindowExW(
		0, // 8
		L"DeusExHRDCE",
		windowName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		nullptr,
		nullptr,
		wndClass.hInstance,
		nullptr);
}

uint32_t g_width = 640;
uint32_t g_height = 480;

void createRenderDevice2(HWND hwnd) {
	if (useDX11 == 0)
		; // createPCRenderDevice(hwnd, g_width, g_height, 0);
	if (useDX11 == 1)
		createPCDX11RenderDevice(hwnd, g_width, g_height, 0);
	// TODO: ShaderUsageDX11.bin
}

bool createRenderDevice() {
	// TODO: decrease ref count on device manager
	createRenderDevice2(hwnd1);
	// TODO: DX11DeviceCallback
	return true;
}

int WinMain2(HINSTANCE hInstance, LPSTR lpCmdLine) {
	loadBuiltinResources(); // specific to this decompile

	uint32_t showSetupOrConfig = 0;
	if (strstr(lpCmdLine, "-setup"))
		showSetupOrConfig = 1;
	if (strstr(lpCmdLine, "-config"))
		showSetupOrConfig = 2;
	#if ENABLE_MCE
		if (strstr(lpCmdLine, "-relaunchmce"))
			relaunchMce = 1;
	#endif

	initDisplayConfig();

	// TODO
	createLowerFileSystems();

	// TODO
	createHigherFileSystems();
	if (archiveFileSystem_default) {
		setupPrefixes(archiveFileSystem_default->getPrefix());
	}

	// TODO
	if (!createDeviceManager()) {
		// TODO: This game requires a DirectX9.0c capable graphics card. Your card or driver does not support DirectX9.0c 3D Acceleration.		
	}
	if (!hasSSE) {
		// TODO: Deus Ex: Human Revolution - Director's Cut can only be played on a PC that supports the SSE instruction set. Please read the readme.rtf or contact Square Enix technical support.
	}
	if (!hasMMX) {
		// TODO: Deus Ex: Human Revolution - Director's Cut can only be played on a PC that supports the MMX instruction set. Please read the readme.rtf or contact Square Enix technical support.
	}

	showSetupIfNecessary(1, showSetupOrConfig);

	createWindow();
	if (!createRenderDevice()) {
		// TODO: Failed to initialize Direct3D with current settings
	}

	*deviceManager->getDisplayConfig() = g_displayConfig; // HACK

	// TODO
	mainloop();

	// TODO
	return 0;
}
