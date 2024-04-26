#pragma once
#include <cstdint>
#include "../../ui/FakeScaleform/fakescaleform.h"
#include "NsTutorialMovieController.h"

class NsTutorialViewerMovieController : public NsTutorialMovieController {
public:
	NsTutorialViewerMovieController(cdc::ScriptType *ty) : NsTutorialMovieController(ty) {}

	int32_t ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs) override;
};
