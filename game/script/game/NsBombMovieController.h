#pragma once
#include <cstdint>
#include "../../ui/FakeScaleform/fakescaleform.h"
#include "NsUberObjectMovieController.h"

class NsBombMovieController : public NsUberObjectMovieController {
public:
	NsBombMovieController(cdc::ScriptType *ty) : NsUberObjectMovieController(ty) {}

	int32_t ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs) override;
};
