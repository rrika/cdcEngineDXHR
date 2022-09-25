#include "ScaleformMovieInstance.h"
#include "../../script/game/NsScaleformMovieController.h"
#include "../../script/game/NtScaleformValue.h"

void ScaleformMovieExternalInterface::Callback(GFxMovieView *pMovieView, const char *methodName, const GFxValue *args, uint32_t numArgs) {
	(void)pMovieView;

	if (mCallback && mCallback(mMovie, methodName, args, numArgs, mUserData) != -1)
		return;

	mMovie->DispatchToMovieControllers(methodName, args, numArgs);
};

ScaleformMovieInstance::ScaleformMovieInstance(ScaleformMovie *movie /*TODO*/) :
	IScaleformMovieInstance(movie /*TODO*/)
{
	// TODO
	m_externalInterface = new ScaleformMovieExternalInterface();
	m_externalInterface->mMovie = this;
	// TODO
	GFxMovieDef::MemoryParams dummyParams;
	m_gfxMovieInstance = m_movie->pMovieDef->CreateInstance(dummyParams, true /*TODO*/);
	m_gfxMovieInstance->SetState(GFxState::StateType::State_ExternalInterface, m_externalInterface);
	// TODO
}

void ScaleformMovieInstance::SetExternalInterfaceRetVal(GFxValue const& val) {
	m_gfxMovieInstance->SetExternalInterfaceRetVal(val);	
}

uint8_t ScaleformMovieInstance::DispatchToMovieControllers(const char *methodName, const GFxValue *args, uint32_t numArgs) {
	// HACK
	for (NsScaleformMovieController *controller: m_controllerArray) {
		cdc::ScriptNativeDynArray<NtScaleformValue> scriptArgs(sizeof(NtScaleformValue));
		scriptArgs.Reserve(numArgs);
		for (uint32_t i=0; i<numArgs; i++)
			scriptArgs[i].m_value = args[i];
		controller->TestPrefixAndProcessMovieFunction(methodName, scriptArgs);
	}
	return 0;
}

void ScaleformMovieInstance::init() {
	m_gfxMovieInstance->init();
}

void ScaleformMovieInstance::buildUI() {
	m_gfxMovieInstance->buildUI();
}
