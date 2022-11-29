#pragma once
#include <cstdint>
#include "../../ui/FakeScaleform/fakescaleform.h"
#include "cdcScript/ScriptDynArray.h"
#include "cdcScript/ScriptObject.h"
#include "NtScaleformValue.h"

class IScaleformMovieInstance;

namespace cdc { struct ScriptSymbol; }

class NsScaleformMovieController : public cdc::ScriptObject {
public:
	IScaleformMovieInstance *movieInstance = nullptr;
public:
	NsScaleformMovieController(cdc::ScriptType *ty) : cdc::ScriptObject(ty) {}

	virtual int32_t TestPrefixAndProcessMovieFunction(const char *name, cdc::ScriptNativeDynArray<NtScaleformValue> const& args); // 4C
	virtual int32_t ProcessMovieFunction(const char *name, GFxValue *args, uint32_t numArgs) = 0; // 50

	int32_t ProcessMovieFunction(cdc::ScriptSymbol& methodName, cdc::ScriptNativeDynArray<NtScaleformValue> const& args);
};
