#include <cstdio>
#include <cstring>
#include "../../ui/Scaleform/ScaleformMovieInstance.h"
#include "NsMainMenuMovieController.h"

int32_t NsMainMenuMovieController::ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs) {
	bool canContinue = false;
	bool isNewGamePlusAvailable = false;
	bool canPlayCredits = false;

	if (strcmp(name, "GetMenuInfo") == 0) {
		puts("TODO: GetMenuInfo");
	} else if (strcmp(name, "GetDLCInfo") == 0) {
		puts("TODO: GetDLCInfo");
	} else if (strcmp(name, "StartNewGame") == 0) {
		puts("TODO: StartNewGame");
	} else if (strcmp(name, "LoadLatestSavedGame") == 0) {
		puts("TODO: LoadLatestSavedGame");
	} else if (strcmp(name, "MapSelector") == 0) {
		puts("TODO: MapSelector");
	} else if (strcmp(name, "IsDLCGameAvailable") == 0) {
		puts("TODO: IsDLCGameAvailable");
	} else if (strcmp(name, "IsNewGamePlusAvailable") == 0) {
		GFxValue val(isNewGamePlusAvailable);
		movieInstance->SetExternalInterfaceRetVal(val);
	} else if (strcmp(name, "LoadQuickSaveGame") == 0) {
		puts("TODO: LoadQuickSaveGame");
	} else if (strcmp(name, "ExitGame") == 0) {
		puts("TODO: ExitGame");
	} else if (strcmp(name, "RequestStorageSelection") == 0) {
		puts("TODO: RequestStorageSelection");
	} else if (strcmp(name, "CanContinue") == 0) {
		GFxValue val(canContinue);
		movieInstance->SetExternalInterfaceRetVal(val);
	} else if (strcmp(name, "CanPlayCredits") == 0) {
		GFxValue val(canPlayCredits);
		movieInstance->SetExternalInterfaceRetVal(val);
	} else if (strcmp(name, "StartCredits") == 0) {
		puts("TODO: StartCredits");
	} else if (strcmp(name, "StartMakingOf") == 0) {
		puts("TODO: StartMakingOf");
	} else if (strcmp(name, "UnlockEndgameTrophy") == 0) {
		puts("TODO: UnlockEndgameTrophy");
	} else if (strcmp(name, "IsLoadAllowed") == 0) {
		puts("TODO: IsLoadAllowed");
	} else if (strcmp(name, "IsOnliveDemo") == 0) {
		puts("TODO: IsOnliveDemo");
	} else if (strcmp(name, "IsStorageSelectionEnabled") == 0) {
		puts("TODO: IsStorageSelectionEnabled");
	} else if (strcmp(name, "LoadNewGamePlus") == 0) {
		puts("TODO: LoadNewGamePlus");
	} else if (strcmp(name, "UnprimeNewGamePlus") == 0) {
		puts("TODO: UnprimeNewGamePlus");
	}
	// TODO
	return 0;
}
