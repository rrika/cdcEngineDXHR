#include <windows.h>
#include <stdio.h>
#include "types.h"
#include "config.h"
#include "main.h"
#include "main2.h"

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
uint WM_DEUS_EX_GCS_FEEDBACK = RegisterWindowMessageA((char const*)L"WM_DEUS_EX_GCS_FEEDBACK");

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
