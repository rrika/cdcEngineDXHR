#pragma once
#include <vector>
// #include "../../../sys/Array.h"
#include "../FakeScaleform/fakescaleform.h"

class NsScaleformMovieController;

// using ScaleformMovieControllerArray = cdc::Array<cdc::GCPtr<NsScaleformMovieController>>;
using ScaleformMovieControllerArray = std::vector<NsScaleformMovieController*>;

class IScaleformMovieInstance {
public:
	// cdc::Handle<ScaleformMovie> m_movie;
	ScaleformMovie *m_movie; // 4
	ScaleformMovieControllerArray m_controllerArray; // 14
public:
	IScaleformMovieInstance(ScaleformMovie *movie /*, TODO*/) : m_movie(movie) { /*TODO*/ }

	// TODO
	virtual void SetExternalInterfaceRetVal(GFxValue const& val) = 0; // 6C
};
