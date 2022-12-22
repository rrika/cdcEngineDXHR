#include "cdcScript/cdcScript.h"
#include "cdcScript/NativeScriptType.h"
#include "cdcScript/ScriptManager.h"
#include "cdcScript/ScriptObject.h"
#include "cdcScript/ScriptSymbol.h"
#include "game/_NsScaleformMovieController.h"
#include "game/script/game/NsScaleformMovieController.h"

using namespace cdc;

int32_t _NsScaleformMovieController::s_virtual = 0;

class NsScaleformMovieController_ScriptType : public NativeScriptType {
public:
	NsScaleformMovieController_ScriptType();
	bool InitType(ScriptType*) override;

	static void _ProcessMovieFunction(void *instance, int32_t numArgs, DataValue *args, void *retVal);
};

NsScaleformMovieController_ScriptType::NsScaleformMovieController_ScriptType() : // line 32520
	NativeScriptType("game/script/game/game", "scaleformmoviecontroller")
{
} 

bool NsScaleformMovieController_ScriptType::InitType(ScriptType *type) { // line 32579

	NativeScriptType::InitType(type);
	InitFunction(0, &_ProcessMovieFunction);
	// InitFunction(1, &_ProcessMovieFunction);
	// InitFunction(2, &_ProcessMovieFunction);
	// TODO

	_NsScaleformMovieController::s_virtual = type->blob->prototypes[0].vtIndex;

	return true;
}

void NsScaleformMovieController_ScriptType::_ProcessMovieFunction( // line 32624
	void *instance, int32_t numArgs, DataValue *args, void *retVal)
{
	auto *controller = (NsScaleformMovieController*) instance;
	*(uint32_t*)retVal = controller->ProcessMovieFunction(
		*(ScriptSymbol*)
			&args[0],
		*(cdc::ScriptNativeDynArray<NtScaleformValue>*)
			args[1].address
	);
}

int32_t _NsScaleformMovieController::ProcessMovieFunction( // line 32685
	ScriptSymbol& methodName,
	ScriptNativeDynArray<NtScaleformValue> const& args)
{
	int32_t retval;
	uint8_t state = m_self->GetState();
	
	DataValue funArgs[2] = {
		{ .stringValue = methodName.m_string },
		{ .address = (uint8_t*)&args }
	};

	auto *func = m_self->GetFunction(state, _NsScaleformMovieController::s_virtual);
	m_self->CallFunction(func, 2, funArgs, &retval);
	return retval;
}

void Init_NativeScripts() { // line 44434
	// TODO
	auto *scriptManager = cdc::ScriptManager::s_instance;
	scriptManager->RegisterNativeScriptType(new NsScaleformMovieController_ScriptType());

}
