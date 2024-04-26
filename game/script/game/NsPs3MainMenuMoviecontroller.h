#pragma once
#include <cstdint>
#include "../../ui/FakeScaleform/fakescaleform.h"
#include "NsMainMenuMovieController.h"

class NsPs3MainMenuMoviecontroller : public NsMainMenuMovieController {
public:
	NsPs3MainMenuMoviecontroller(cdc::ScriptType *ty) : NsMainMenuMovieController(ty) {}

	int32_t ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs) override;
};
