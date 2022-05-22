#pragma once
#include <cstdint>
#include "AnchoredCamera.h"
#include "AutoSplineCamera.h"
#include "CameraDrawWrapper.h"
#include "CinematicCamera.h"
#include "FreeCamera.h"
#include "ICameraManager.h"
#include "ModelViewCamera.h"
#include "ObjectDebugCamera.h"
#include "OrbitDebugCamera.h"
#include "PlayerCamera.h"
#include "TransitionCamera.h"

struct CameraManagerSub1464 {
	uint32_t dword0;
	uint32_t dword4;
	uint32_t dword8;
	uint8_t byteC;
	uint8_t byteD;
};

class CameraManager : public cdc::ICameraManager {
public:
	CameraDrawWrapper<ObjectDebugCamera> objectDebugCamera; // 60
	CameraDrawWrapper<TransitionCamera> transitionCamera; // 150
	CameraDrawWrapper<AnchoredCamera> anchoredCamera; // 220
	CinematicCamera cinematicCamera; // 340
	CameraDrawWrapper<AutoSplineCamera> autosplineCamera; // 490
	CameraDrawWrapper<FreeCamera> freeCamera; // 550
	CameraDrawWrapper<ModelViewCamera> modelviewCamera; // 630
	PlayerCamera playerCamera; // 6F0
	OrbitDebugCamera orbitDebugCamera; // 11A0
	CameraManagerSub1464 sub1464;
};
