#pragma once
#include <cstdint>
#include "../../ui/FakeScaleform/fakescaleform.h"
#include "NsScaleformMovieController.h"

class NsMainMenuMovieController : public NsScaleformMovieController {
public:
	uint32_t ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs) override;
};
