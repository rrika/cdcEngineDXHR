#pragma once
#include <cstdint>
#include "ScaleformMovie.h" // for ScaleformCallback
#include "IScaleformMovieInstance.h"
#include "../FakeScaleform/fakescaleform.h"

class ScaleformMovieExternalInterface : public GFxExternalInterface {
public:
	ScaleformCallback *mCallback = nullptr; // C
	ScaleformMovieInstance *mMovie; // 10
	void *mUserData; // 14

public:
	void Callback(GFxMovieView *pMovieView, const char *methodName, const GFxValue *args, uint32_t numArgs) override;
};

class ScaleformMovieInstance : public IScaleformMovieInstance {
public:
	// GPtr<ScaleformMovieExternalInterface> m_externalInterface;
	ScaleformMovieExternalInterface *m_externalInterface; // 44
	// GPtr<GFxMovieView> m_gfxMovieInstance; // 4C
	GFxMovieView *m_gfxMovieInstance; // 4C

	ScaleformMovieInstance(ScaleformMovie *movie/*TODO*/);
	~ScaleformMovieInstance();
	uint8_t DispatchToMovieControllers(const char *methodName, const GFxValue *args, uint32_t numArgs);
	void init(); // HACK
	void buildUI(); // HACK

	void SetExternalInterfaceRetVal(GFxValue const& val) override;
};
