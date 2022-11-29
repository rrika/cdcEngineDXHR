#pragma once
#include <cstdint>

namespace cdc {
class ScriptObject;
struct ScriptSymbol;
template <typename T> class ScriptNativeDynArray;
}

struct NtScaleformValue;

struct _NsScaleformMovieController {
	static int32_t s_virtual;
	cdc::ScriptObject *m_self;

	int32_t ProcessMovieFunction(
		cdc::ScriptSymbol& methodName,
		cdc::ScriptNativeDynArray<NtScaleformValue> const& args);
};
