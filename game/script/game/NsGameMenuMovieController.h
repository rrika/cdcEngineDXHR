#pragma once
#include <cstdint>
#include "../../ui/FakeScaleform/fakescaleform.h"
#include "NsScaleformMovieController.h"

class NsGameMenuMovieController : public NsScaleformMovieController {
public:
	NsGameMenuMovieController(cdc::ScriptType *ty) : NsScaleformMovieController(ty) {}

	int32_t ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs) override;
};
