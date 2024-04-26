#pragma once
#include <cstdint>
#include "../../ui/FakeScaleform/fakescaleform.h"
#include "NsCinematicPauseMenuMovieController.h"

class NsVideoCinematicPauseMenuMovieController : public NsCinematicPauseMenuMovieController {
public:
	NsVideoCinematicPauseMenuMovieController(cdc::ScriptType *ty) : NsCinematicPauseMenuMovieController(ty) {}

	int32_t ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs) override;
};
