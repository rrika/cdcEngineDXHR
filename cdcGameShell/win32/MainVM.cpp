#include <cstdio>
#include <windows.h>
#include "config.h"
#include "cdcFile/ArchiveFileSystem.h"
#include "cdcFile/FileHelpers.h"
#include "../../game/Main.h"
#include "cdcGameShell/win32/gamewindow.h"
#include "imgui/imgui.h"
#include "cdcLocale/localstr.h"
#include "../../rendering/BuiltinResources.h"
#include "../../rendering/IPCDeviceManager.h"
#include "../../rendering/PCDX11DeviceManager.h"
#include "../../rendering/PCDX11RenderDevice.h"
#include "../../rendering/renderdevice.h"

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

IPCDeviceManager *getDeviceManager() {
	if (g_CurrentRenderer == RENDERER_DX9)
		return deviceManager9;
	if (g_CurrentRenderer == RENDERER_DX11)
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

DisplayConfig displayConfig;

uint32_t g_width = 640;
uint32_t g_height = 480;

void createRenderDevice2(HWND hwnd) {
	// HACK
	RECT rect;
	GetClientRect(hwnd, &rect);
	g_width = rect.right;
	g_height = rect.bottom;

	if (g_CurrentRenderer == RENDERER_DX9)
		; // createPCRenderDevice(hwnd, g_width, g_height, 0);
	if (g_CurrentRenderer == RENDERER_DX11)
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
		SetupBuildDir(archiveFileSystem_default->getPrefix());
	}

	localstr_reload(); // HACK

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

	GameWindow::Create();
	if (!createRenderDevice()) {
		// TODO: Failed to initialize Direct3D with current settings
	}

	*deviceManager->getDisplayConfig() = g_displayConfig; // HACK

	// TODO
	MAIN_Init();

	// TODO
	return 0;
}

bool IsAlreadyRunning() {
	HANDLE mutex = CreateMutexW(0, 0, L"DXHRDCEMUTEX");
	if (!mutex)
		return true;

	bool alreadyRunning = GetLastError() == ERROR_ALREADY_EXISTS;
	CloseHandle(mutex);
	return alreadyRunning;
}

#if ENABLE_MCE
bool relaunchMce = false;
void LaunchWindowsMediaCenter() {
	wchar_t path[MAX_PATH]; // 260
	if (GetSystemMetrics(SM_MEDIACENTER) &&
		ExpandEnvironmentStringsW(L"%SystemRoot%\\ehome\\ehshell.exe", path, MAX_PATH) &&
		GetFileAttributesW(path) != -1)
	{
		ShellExecuteW(0, L"open", path, 0, 0, 1);
	}
}
#endif

#if ENABLE_GCS

int intelFeedbackNumbers[5];
uint32_t WM_DEUS_EX_GCS_FEEDBACK = RegisterWindowMessageA((char const*)L"WM_DEUS_EX_GCS_FEEDBACK");

void SetIntelFeedbackNumbers(int a, int b, int c, int d, int e) {
	intelFeedbackNumbers[0] = a;
	intelFeedbackNumbers[1] = b;
	intelFeedbackNumbers[2] = c;
	intelFeedbackNumbers[3] = d;
	intelFeedbackNumbers[4] = e;
}

inline bool IntelFeedbackFromCmdline(LPSTR lpCmdLine) {
	int numbers[5] = {0, 0, 0, 0, 0};
	char *intel_feedback = strstr(lpCmdLine, "intel_feedback=");
	if (intel_feedback && sscanf_s(intel_feedback + 15, "%d.%d.%d.%d:%d",
		numbers + 0,
		numbers + 1,
		numbers + 2,
		numbers + 3,
		numbers + 4) == 5 )
	{
		LPARAM lparam = 0;
		if ( IsAlreadyRunning() )
		{

			SendMessageW(HWND_BROADCAST, WM_DEUS_EX_GCS_FEEDBACK,
				numbers[0] | ((numbers[1] | ((numbers[2] | (numbers[3] << 8)) << 8)) << 8),
				numbers[4]);
			return true;
		}
		SetIntelFeedbackNumbers(numbers[0], numbers[1], numbers[2], numbers[3], numbers[4]);
	}
	return false;
}

#endif

extern "C" int _set_SSE2_enable(int);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	#if ENABLE_GCS
	if (IntelFeedbackFromCmdline(lpCmdLine))
		return 0;
	#endif

	_set_SSE2_enable(0);

	#if ENABLE_STEAM
	if (SteamAPI_RestartAppIfNecessary(238010))
		return 0;
	#endif

	int exitcode = WinMain2(hInstance, lpCmdLine);

	#if ENABLE_MCE
	if (relaunchMce)
		LaunchWindowsMediaCenter();
	#endif

	return exitcode;
}
