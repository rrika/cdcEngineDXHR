#pragma once
#include <cstdint>
#include "../../ui/FakeScaleform/fakescaleform.h"
#include "NsSelectionBoxMovieController.h"

class NsSelectionBoxMinimalMovieController : public NsSelectionBoxMovieController {
public:
	NsSelectionBoxMinimalMovieController(cdc::ScriptType *ty) : NsSelectionBoxMovieController(ty) {}

	int32_t ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs) override;
};
