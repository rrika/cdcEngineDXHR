#pragma once
#include "../FakeScaleform/fakescaleform.h"

class ScaleformMovieInstance;

using ScaleformCallback = int(ScaleformMovieInstance*, const char*, GFxValue const*, unsigned, void*);

class ScaleformMovie {
public:
	// GPtr<GFxMovieDef> pMovieDef; // 20
	GFxMovieDef *pMovieDef; // 20

	ScaleformMovie(GFxMovieDef *pMovieDef) : pMovieDef(pMovieDef) {} // HACK
};
