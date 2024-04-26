#pragma once
#include <cstdint>
#include "../../ui/FakeScaleform/fakescaleform.h"
#include "NsScaleformMovieController.h"

class NsLoginMovieController : public NsScaleformMovieController {
public:
	NsLoginMovieController(cdc::ScriptType *ty) : NsScaleformMovieController(ty) {}

	int32_t ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs) override;
};
