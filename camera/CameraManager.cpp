#include "CameraManager.h"
#include "cdcSys/Assert.h"

CameraManager::CameraManager() {

	cameras[0] = nullptr;            //  4
	cameras[1] = &playerCamera;      //  8, stack[..] = 1
	cameras[2] = &transitionCamera;  //  C, stack[..] = 2
	cameras[3] = &anchoredCamera;    // 10
	cameras[4] = &freeCamera;        // 14, stack[..] = 3
	cameras[5] = &modelviewCamera;   // 18, stack[..] = 4
	cameras[6] = &cinematicCamera;   // 1C, stack[..] = 5
	cameras[7] = &autosplineCamera;  // 20
	cameras[8] = nullptr;            // 24
	cameras[9] = &objectDebugCamera; // 28, stack[..] = 7
	cameras[10] = &orbitDebugCamera; // 2C, stack[..] = 8

	activeCamera = &playerCamera;
	activeCameraIndex = 1;
	sp = 0;
	stack[sp++] = 1;
}

cdc::ICamera *CameraManager::getStackCamera(uint32_t i) {
	switch (i) {
	case 0: return cameras[activeCameraIndex];
	case 1: return &playerCamera;      // cameras[1]
	case 2: return &transitionCamera;  // cameras[2]
	case 3: return &freeCamera;        // cameras[4]
	case 4: return &modelviewCamera;   // cameras[5]
	case 5: return &cinematicCamera;   // cameras[6]
	case 7: return &objectDebugCamera; // cameras[9]
	case 8: return &orbitDebugCamera;  // cameras[10]
	default:
		cdc::FatalError("CameraManager: invalid mode %d", i);
		return nullptr;
	}
}

void CameraManager::switchTo(cdc::ICamera *nextCamera) {
	auto *prevCamera = activeCamera;
	prevCamera->disable(nextCamera);
	activeCamera = nextCamera;
	nextCamera->enable(prevCamera);
}

void CameraManager::update() {
	matrix = *activeCamera->getMatrix();
}

void CameraManager::pushFreeCamera() {
	switchTo(&freeCamera);
	stack[sp++] = 3;
}

void CameraManager::popFreeCamera() {
	stack[--sp] = 0;
	switchTo(getStackCamera(stack[sp-1]));
}

void CameraManager::pushModelViewCamera() {
	switchTo(&modelviewCamera);
	stack[sp++] = 4;
}

void CameraManager::popModelViewCamera() {
	stack[--sp] = 0;
	switchTo(getStackCamera(stack[sp-1]));
}

void CameraManager::pushCinematicCamera() {
	switchTo(&cinematicCamera);
	stack[sp++] = 5;
}

void CameraManager::popCinematicCamera() {
	// TODO
	stack[--sp] = 0;
	switchTo(getStackCamera(stack[sp-1]));
}

void CameraManager::pushObjectDebugCamera() {
	switchTo(&objectDebugCamera);
	stack[sp++] = 7;
}

void CameraManager::popObjectDebugCamera() {
	stack[--sp] = 0;
	switchTo(getStackCamera(stack[sp-1]));
}

void CameraManager::pushOrbitDebugCamera() {
	switchTo(&orbitDebugCamera);
	stack[sp++] = 8;
}

void CameraManager::popOrbitDebugCamera() {
	stack[--sp] = 0;
	switchTo(getStackCamera(stack[sp-1]));
}

bool CameraManager::isPlayerCamera() {
	return stack[sp-1] == 1;
}

bool CameraManager::isFreeCamera() {
	return stack[sp-1] == 3;
}

bool CameraManager::isModelViewCamera() {
	return stack[sp-1] == 4;
}

bool CameraManager::isCinematicCamera() {
	return stack[sp-1] == 5;
}

bool CameraManager::isTransitionCamera() {
	return stack[sp-1] == 2;
}

cdc::Matrix *CameraManager::getActiveCameraMatrix() {
	return activeCamera->getMatrix();
}

cdc::Matrix *CameraManager::getMatrix() {
	return &matrix;
}
