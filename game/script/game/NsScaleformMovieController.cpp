#include "NsScaleformMovieController.h"
#include "cdcScript/ScriptSymbol.h"
#include "code/game/script/game/_NsScaleformMovieController.h"

int32_t NsScaleformMovieController::TestPrefixAndProcessMovieFunction(
	const char *methodName,
	cdc::ScriptNativeDynArray<NtScaleformValue> const& args)
{
	cdc::ScriptSymbol methodSymbol { methodName };

	if (false) { // bypass scripting system
		return ProcessMovieFunction(methodSymbol, args);
	} else {
		// TODO
		_NsScaleformMovieController wrapper { this };
		return wrapper.ProcessMovieFunction(methodSymbol, args);
	}
}

int32_t NsScaleformMovieController::ProcessMovieFunction(cdc::ScriptSymbol& methodName, cdc::ScriptNativeDynArray<NtScaleformValue> const& scriptArgs) {
	uint32_t numArgs = scriptArgs.m_size;
	GFxValue *args = new GFxValue[numArgs];
	for (uint32_t i=0; i<numArgs; i++)
		args[i] = scriptArgs[i].m_value;
	int32_t r = ProcessMovieFunction(methodName.m_string, args, numArgs);
	delete[] args;
	return r;
}
