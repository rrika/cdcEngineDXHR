#pragma once
#include <cstdint>
#include "cdcMath/Math.h"
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
	cdc::ICamera *cameras[11]; // 4
	cdc::ICamera *activeCamera; // 30
	uint32_t activeCameraIndex; // 34
	uint32_t stack[8]; // 38
	uint32_t sp; // 58

	CameraDrawWrapper<ObjectDebugCamera> objectDebugCamera; // 60
	CameraDrawWrapper<TransitionCamera> transitionCamera; // 150
	CameraDrawWrapper<AnchoredCamera> anchoredCamera; // 220
	CinematicCamera cinematicCamera; // 340
	CameraDrawWrapper<AutoSplineCamera> autosplineCamera; // 490
	CameraDrawWrapper<FreeCamera> freeCamera; // 550
	CameraDrawWrapper<ModelViewCamera> modelviewCamera; // 630
	PlayerCamera playerCamera; // 6F0
	OrbitDebugCamera orbitDebugCamera; // 11A0
	// CameraManagerSub1464 sub1464;

	cdc::Matrix matrix; // 13B0

	CameraManager();

	void update() override; // 8

	void pushFreeCamera() override; // 24
	void popFreeCamera() override; // 28
	void pushModelViewCamera() override; // 2C
	void popModelViewCamera() override; // 30
	void pushCinematicCamera() override; // 34
	void popCinematicCamera() override; // 38
	void pushObjectDebugCamera() override; // 3C
	void popObjectDebugCamera() override; // 40
	void pushOrbitDebugCamera() override; // 44
	void popOrbitDebugCamera() override; // 48

	bool isPlayerCamera() override; // 4C
	bool isFreeCamera() override; // 50
	bool isModelViewCamera() override; // 54
	bool isCinematicCamera() override; // 58
	bool isTransitionCamera() override; // 5C

	cdc::Matrix *getActiveCameraMatrix() override; // D4
	cdc::Matrix *getMatrix() override; // D8

protected:
	cdc::ICamera *getStackCamera(uint32_t);
	void switchTo(cdc::ICamera *nextCamera);
};
