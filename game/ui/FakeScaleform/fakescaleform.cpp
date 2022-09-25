#include "fakescaleform.h"

void GFxMovieRoot::SetExternalInterfaceRetVal(const GFxValue& value) {
	lastReturnValue = value;
}

void GFxMovieRoot::SetState(GFxState::StateType state, GFxState* pstate) {
	if (state == GFxState::State_ExternalInterface)
		externalInterface = static_cast<GFxExternalInterface*>(pstate);
}

GFxMovieView *GFxMovieDefImpl::CreateInstance(GFxMovieDef::MemoryParams const& memParams, bool initFirstFrame) {
	return createInstance();
}
