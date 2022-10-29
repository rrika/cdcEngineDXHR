#pragma once
#include <cstdint>
#include "../../ui/FakeScaleform/fakescaleform.h"
#include "cdcScript/ScriptDynArray.h"
#include "NtScaleformValue.h"

class IScaleformMovieInstance;

class NsScaleformMovieController {
public:
	IScaleformMovieInstance *movieInstance = nullptr;
public:
	virtual uint32_t TestPrefixAndProcessMovieFunction(const char *name, cdc::ScriptNativeDynArray<NtScaleformValue> const& args); // 4C
	virtual uint32_t ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs) = 0; // 50
};
