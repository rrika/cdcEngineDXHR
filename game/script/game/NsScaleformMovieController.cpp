#include "NsScaleformMovieController.h"

uint32_t NsScaleformMovieController::TestPrefixAndProcessMovieFunction(
	const char *methodName,
	cdc::ScriptNativeDynArray<NtScaleformValue> const& scriptArgs)
{
	// HACK
	uint32_t numArgs = scriptArgs.m_size;
	GFxValue *args = new GFxValue[numArgs];
	for (uint32_t i=0; i<numArgs; i++)
		args[i] = scriptArgs[i].m_value;
	uint32_t r = ProcessMovieFunction(methodName, args, numArgs);
	delete[] args;
	return r;
}
